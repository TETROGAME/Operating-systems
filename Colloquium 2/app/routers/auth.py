from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.orm import Session

from app.core.db import get_db
from app.models.user import User
from app.schemas.auth import RegisterInput, LoginInput, TokenOut
from app.security.auth import security, hash_password, verify_password_and_update
from app.security.deps import get_current_user

router = APIRouter(prefix="/auth", tags=["auth"])

@router.post("/register")
def register(payload: RegisterInput, db: Session = Depends(get_db)):
    existing = db.query(User).filter(User.username == payload.username).first()
    if existing:
        raise HTTPException(status_code=400, detail="User already exists")
    user = User(username=payload.username, hashed_password=hash_password(payload.password), disabled=False)
    db.add(user)
    db.commit()
    db.refresh(user)
    return {"id": user.id, "username": user.username}

@router.post("/login", response_model=TokenOut)
def login(payload: LoginInput, db: Session = Depends(get_db)):
    user = db.query(User).filter(User.username == payload.username).first()
    if not user:
        raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED, detail="Invalid credentials")

    valid, new_hash = verify_password_and_update(payload.password, user.hashed_password)
    if not valid:
        raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED, detail="Invalid credentials")

    if new_hash:
        user.hashed_password = new_hash
        db.add(user)
        db.commit()

    if user.disabled:
        raise HTTPException(status_code=status.HTTP_403_FORBIDDEN, detail="User is disabled")

    access_token = security.create_access_token(uid=user.username)
    return TokenOut(access_token=access_token)

@router.get("/me")
def me(current_user: User = Depends(get_current_user)):
    return {"id": current_user.id, "username": current_user.username, "disabled": current_user.disabled}