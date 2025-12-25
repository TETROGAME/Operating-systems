from datetime import datetime
from sqlalchemy import Integer, String, Text, DateTime, Enum as SAEnum, func
from sqlalchemy.orm import Mapped, mapped_column

from app.storage.db import Base
from app.schemas import Status

class Task(Base):
    __tablename__ = "tasks"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, autoincrement=True)
    title: Mapped[str] = mapped_column(String(200), nullable=False)
    description: Mapped[str | None] = mapped_column(Text, nullable=True)
    status: Mapped[Status] = mapped_column(
        SAEnum(Status, name="task_status", native_enum=True),
        nullable=False,
        default=Status.todo,
    )

    created_at: Mapped[datetime | None] = mapped_column(DateTime(timezone=True), server_default=func.now(), nullable=True)
    updated_at: Mapped[datetime | None] = mapped_column(DateTime(timezone=True), server_default=func.now(), onupdate=func.now(), nullable=True)