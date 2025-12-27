from typing import List
from sqlalchemy.orm import Session
from fastapi import HTTPException, status

from app.models.task_models import TaskCreate, TaskUpdate, TaskOut
from app.storage.models import Task as TaskModel

def list_tasks(db: Session) -> List[TaskOut]:
    rows = db.query(TaskModel).order_by(TaskModel.id.asc()).all()
    return [TaskOut.model_validate(r, from_attributes=True) for r in rows]

def create_task(db: Session, data: TaskCreate) -> TaskOut:
    row = TaskModel(**data.model_dump())
    db.add(row)
    db.commit()
    db.refresh(row)
    return TaskOut.model_validate(row, from_attributes=True)

def get_task(db: Session, task_id: int) -> TaskOut:
    row = db.get(TaskModel, task_id)
    if not row:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    return TaskOut.model_validate(row, from_attributes=True)

def put_task(db: Session, task_id: int, data: TaskCreate) -> TaskOut:
    row = db.get(TaskModel, task_id)
    if not row:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    row.title = data.title
    row.description = data.description
    row.status = data.status
    db.commit()
    db.refresh(row)
    return TaskOut.model_validate(row, from_attributes=True)

def patch_task(db: Session, task_id: int, data: TaskUpdate) -> TaskOut:
    row = db.get(TaskModel, task_id)
    if not row:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    payload = data.model_dump(exclude_unset=True)
    for field, value in payload.items():
        setattr(row, field, value)
    db.commit()
    db.refresh(row)
    return TaskOut.model_validate(row, from_attributes=True)

def delete_task(db: Session, task_id: int) -> None:
    row = db.get(TaskModel, task_id)
    if not row:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    db.delete(row)
    db.commit()