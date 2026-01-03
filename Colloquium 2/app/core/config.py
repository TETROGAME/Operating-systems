import os

from dotenv import load_dotenv

load_dotenv()

DATABASE_URL = os.getenv("DATABASE_URL", "sqlite:///./data/app.db")

JWT_SECRET_KEY = os.getenv("JWT_SECRET_KEY", "TEST_SECRET_KEY")
JWT_ALGORITHM = os.getenv("JWT_ALGORITHM", "HS256")
JWT_ACCESS_TOKEN_EXPIRES_IN_MINUTES = int(os.getenv("JWT_ACCESS_TOKEN_EXPIRES_IN_MINUTES", "15"))

REDIS_URL = os.getenv("REDIS_URL", "redis://localhost:6379/0")
CACHE_TASK_TTL = int(os.getenv("CACHE_TASK_TTL", "300"))

RQ_REDIS_URL = os.getenv("RQ_REDIS_URL", "redis://localhost:6379/3")

if not JWT_SECRET_KEY or JWT_SECRET_KEY == "TEST_SECRET_KEY":
    raise RuntimeError("JWT_SECRET_KEY is not set or too weak. Put a strong value in .env or environment.")