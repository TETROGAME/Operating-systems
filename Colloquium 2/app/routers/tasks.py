from typing import List
from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.orm import Session

from app.core.db import get_db
from app.models.task import Task, Status as SAStatus
from app.schemas.task import TaskCreate, TaskUpdate, TaskOut
from app.security.deps import get_current_user_id

router = APIRouter(prefix="/tasks", tags=["tasks"])

@router.get("", response_model=List[TaskOut])
def list_tasks(db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    rows = db.query(Task).filter(Task.user_id == user_id).order_by(Task.id.asc()).all()
    return [TaskOut.model_validate(r) for r in rows]

@router.post("", response_model=TaskOut, status_code=status.HTTP_201_CREATED)
def create_task(data: TaskCreate, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    row = Task(title=data.title, description=data.description, status=SAStatus(data.status.value), user_id=user_id)
    db.add(row)
    db.commit()
    db.refresh(row)
    return TaskOut.model_validate(row)

@router.get("/{task_id}", response_model=TaskOut)
def get_task(task_id: int, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    row = db.query(Task).filter(Task.id == task_id, Task.user_id == user_id).first()
    if not row:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    return TaskOut.model_validate(row)

@router.put("/{task_id}", response_model=TaskOut)
def put_task(task_id: int, data: TaskCreate, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    row = db.query(Task).filter(Task.id == task_id, Task.user_id == user_id).first()
    if not row:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    row.title = data.title
    row.description = data.description
    row.status = SAStatus(data.status.value)
    db.commit()
    db.refresh(row)
    return TaskOut.model_validate(row)

@router.patch("/{task_id}", response_model=TaskOut)
def patch_task(task_id: int, data: TaskUpdate, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    row = db.query(Task).filter(Task.id == task_id, Task.user_id == user_id).first()
    if not row:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    payload = data.model_dump(exclude_unset=True)
    if "title" in payload:
        row.title = payload["title"]
    if "description" in payload:
        row.description = payload["description"]
    if "status" in payload and payload["status"] is not None:
        row.status = SAStatus(payload["status"].value)
    db.commit()
    db.refresh(row)
    return TaskOut.model_validate(row)

@router.delete("/{task_id}", status_code=status.HTTP_204_NO_CONTENT)
def delete_task(task_id: int, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    row = db.query(Task).filter(Task.id == task_id, Task.user_id == user_id).first()
    if not row:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    db.delete(row)
    db.commit()
    return None