from typing import List
from fastapi import APIRouter, status, Depends
from sqlalchemy.orm import Session

from app.schemas import TaskCreate, TaskUpdate, TaskOut
from app.crud.tasks import (
    list_tasks,
    create_task,
    get_task,
    put_task,
    patch_task,
    delete_task,
)
from app.storage.db import get_db

router = APIRouter(prefix="/tasks", tags=["tasks"])

@router.get("", response_model=List[TaskOut], summary="Get all tasks")
def list_tasks_endpoint(db: Session = Depends(get_db)):
    return list_tasks(db)

@router.post("", response_model=TaskOut, status_code=status.HTTP_201_CREATED, summary="Create new task")
def create_task_endpoint(payload: TaskCreate, db: Session = Depends(get_db)):
    return create_task(db, payload)

@router.get("/{task_id}", response_model=TaskOut, summary="Get task via ID")
def get_task_endpoint(task_id: int, db: Session = Depends(get_db)):
    return get_task(db, task_id)

@router.put("/{task_id}", response_model=TaskOut, summary="Update whole task via ID")
def put_task_endpoint(task_id: int, payload: TaskCreate, db: Session = Depends(get_db)):
    return put_task(db, task_id, payload)

@router.patch("/{task_id}", response_model=TaskOut, summary="Update task partially via ID")
def patch_task_endpoint(task_id: int, payload: TaskUpdate, db: Session = Depends(get_db)):
    return patch_task(db, task_id, payload)

@router.delete("/{task_id}", status_code=status.HTTP_204_NO_CONTENT, summary="Delete task via ID")
def delete_task_endpoint(task_id: int, db: Session = Depends(get_db)):
    delete_task(db, task_id)
    return