from typing import Any
from fastapi import Depends, HTTPException, status
from sqlalchemy.orm import Session

from app.core.db import get_db
from app.security.auth import security
from app.models.user import User

RequireAccessToken = security.access_token_required

def _extract_subject(payload: Any) -> str | None:
    uid = getattr(payload, "uid", None) or getattr(payload, "sub", None)
    if uid is None and isinstance(payload, dict):
        uid = payload.get("uid") or payload.get("sub")
    return uid

def get_current_user(
    payload: Any = Depends(RequireAccessToken),
    db: Session = Depends(get_db),
) -> User:
    uid = _extract_subject(payload)
    if not uid:
        raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED, detail="Token missing subject")

    user = db.query(User).filter(User.username == uid).first()
    if not user:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="User not found")
    if user.disabled:
        raise HTTPException(status_code=status.HTTP_403_FORBIDDEN, detail="User is disabled")
    return user

def get_current_user_id(user: User = Depends(get_current_user)) -> int:
    return user.id