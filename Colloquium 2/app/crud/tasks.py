from typing import List, Optional

from fastapi import HTTPException, status

from app.schemas import TaskCreate, TaskUpdate, TaskOut
from app.storage.memory import storage

def list_tasks() -> List[TaskOut]:
    return storage.list()

def create_task(data: TaskCreate) -> TaskOut:
    return storage.create(data)

def get_task(task_id: int) -> TaskOut:
    task = storage.get(task_id)
    if not task:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    return task

def put_task(task_id: int, data: TaskCreate) -> TaskOut:
    updated = storage.replace(task_id, data)
    if not updated:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    return updated

def patch_task(task_id: int, data: TaskUpdate) -> TaskOut:
    updated = storage.update(task_id, data)
    if not updated:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    return updated

def delete_task(task_id: int) -> None:
    ok = storage.delete(task_id)
    if not ok:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")