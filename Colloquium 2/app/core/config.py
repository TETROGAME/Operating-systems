import os
from dotenv import load_dotenv

load_dotenv()

DATABASE_URL = os.getenv("DATABASE_URL", "postgresql+psycopg2://todo:todo@127.0.0.1:5432/todo")

JWT_SECRET_KEY = os.getenv("JWT_SECRET_KEY", "")
JWT_ALGORITHM = os.getenv("JWT_ALGORITHM", "HS256")
JWT_ACCESS_TOKEN_EXPIRES_IN_MINUTES = int(os.getenv("JWT_ACCESS_TOKEN_EXPIRES_IN_MINUTES", "15"))

if not JWT_SECRET_KEY or JWT_SECRET_KEY in ("CHANGE_ME", "change_me", "secret", "TEST_SECRET_KEY"):
    raise RuntimeError("JWT_SECRET_KEY is not set or too weak. Put a strong value in .env or environment.")