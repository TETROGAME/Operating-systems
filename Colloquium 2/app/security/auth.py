from datetime import timedelta
from typing import Tuple
from passlib.context import CryptContext
from passlib.exc import UnknownHashError

from authx import AuthX, AuthXConfig
from app.core.config import JWT_SECRET_KEY, JWT_ALGORITHM, JWT_ACCESS_TOKEN_EXPIRES_IN_MINUTES

pwd_context = CryptContext(
    schemes=["bcrypt_sha256", "bcrypt"],
    deprecated="auto",
)

def hash_password(password: str) -> str:
    return pwd_context.hash(password)

def verify_password(plain_password: str, hashed_password: str) -> bool:
    if not hashed_password:
        return False
    try:
        return pwd_context.verify(plain_password, hashed_password)
    except UnknownHashError:
        return False

def verify_password_and_update(plain_password: str, hashed_password: str) -> Tuple[bool, str | None]:
    if not hashed_password:
        return False, None
    try:
        valid, new_hash = pwd_context.verify_and_update(plain_password, hashed_password)
        return valid, new_hash
    except UnknownHashError:
        return False, None

security = AuthX(
    config=AuthXConfig(
        JWT_SECRET_KEY=JWT_SECRET_KEY,
        JWT_ALGORITHM=JWT_ALGORITHM,
        JWT_ACCESS_TOKEN_EXPIRES=timedelta(minutes=JWT_ACCESS_TOKEN_EXPIRES_IN_MINUTES)
    )
)

