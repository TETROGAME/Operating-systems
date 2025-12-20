from enum import Enum
from typing import Optional
from datetime import datetime
from pydantic import BaseModel, Field, ConfigDict

class Status(str, Enum):
    todo = "todo"
    in_progress = "in_progress"
    done = "done"

class TaskBase(BaseModel):
    title: str = Field(min_length=1, max_length=200)
    description: Optional[str] = None
    status: Status = Status.todo

class TaskCreate(TaskBase):
    pass

class TaskUpdate(TaskBase):
    title: Optional[str] = Field(None, min_length=1, max_length=200)
    description: Optional[str] = None
    status: Optional[Status] = None

class TaskOut(TaskBase):
    id: int
    title: str
    description: Optional[str] = None
    status: Status
    created_at: Optional[datetime] = None
    updated_at: Optional[datetime] = None