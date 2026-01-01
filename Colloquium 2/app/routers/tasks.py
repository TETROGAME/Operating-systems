import logging
from typing import List

from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.orm import Session

from app.core.db import get_db
from app.core.cache import (
    cache_get_json,
    cache_set_json,
    cache_delete,
    make_task_list_key,
    make_task_key,
)
from app.models.task import Task, Status as SAStatus
from app.schemas.task import TaskCreate, TaskUpdate, TaskOut
from app.security.deps import get_current_user_id
from app.tasks.jobs import enqueue_task_event

router = APIRouter(prefix="/tasks", tags=["tasks"])
logger = logging.getLogger(__name__)

@router.get("", response_model=List[TaskOut])
def list_tasks(db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    cache_key = make_task_list_key(user_id)
    cached = cache_get_json(cache_key)
    if cached is not None:
        logger.info("tasks list (cache hit)", extra={"user_id": user_id})
        return cached
    rows = db.query(Task).filter(Task.user_id == user_id).order_by(Task.id.asc()).all()
    payload = [TaskOut.model_validate(r).model_dump(mode="json") for r in rows]
    cache_set_json(cache_key, payload)
    logger.info("tasks list (db)", extra={"user_id": user_id, "count": len(payload)})
    return payload

@router.post("", response_model=TaskOut, status_code=status.HTTP_201_CREATED)
def create_task(data: TaskCreate, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    row = Task(title=data.title, description=data.description, status=SAStatus(data.status.value), user_id=user_id)
    db.add(row)
    db.commit()
    db.refresh(row)
    task_out = TaskOut.model_validate(row).model_dump(mode="json")
    cache_set_json(make_task_key(user_id, row.id), task_out)
    cache_delete(make_task_list_key(user_id))
    enqueue_task_event("task_created", user_id, row.id, {"title": row.title, "status": row.status.value})
    logger.info("task created", extra={"user_id": user_id, "task_id": row.id})
    return task_out

@router.get("/{task_id}", response_model=TaskOut)
def get_task(task_id: int, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    cache_key = make_task_key(user_id, task_id)
    cached = cache_get_json(cache_key)
    if cached is not None:
        logger.info("task get (cache hit)", extra={"user_id": user_id, "task_id": task_id})
        return cached
    row = db.query(Task).filter(Task.id == task_id, Task.user_id == user_id).first()
    if not row:
        logger.warning("task not found", extra={"user_id": user_id, "task_id": task_id})
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    task_out = TaskOut.model_validate(row).model_dump(mode="json")
    cache_set_json(cache_key, task_out)
    enqueue_task_event("task_fetched", user_id, task_id, {"title": row.title, "status": row.status.value})
    logger.info("task get (db)", extra={"user_id": user_id, "task_id": task_id})
    return task_out

@router.put("/{task_id}", response_model=TaskOut)
def put_task(task_id: int, data: TaskCreate, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    row = db.query(Task).filter(Task.id == task_id, Task.user_id == user_id).first()
    if not row:
        logger.warning("task not found (put)", extra={"user_id": user_id, "task_id": task_id})
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    row.title = data.title
    row.description = data.description
    row.status = SAStatus(data.status.value)
    db.commit()
    db.refresh(row)
    task_out = TaskOut.model_validate(row).model_dump(mode="json")
    cache_set_json(make_task_key(user_id, task_id), task_out)
    cache_delete(make_task_list_key(user_id))
    enqueue_task_event("task_updated", user_id, task_id, {"title": row.title, "status": row.status.value})
    logger.info("task updated", extra={"user_id": user_id, "task_id": task_id})
    return task_out

@router.patch("/{task_id}", response_model=TaskOut)
def patch_task(task_id: int, data: TaskUpdate, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    row = db.query(Task).filter(Task.id == task_id, Task.user_id == user_id).first()
    if not row:
        logger.warning("task not found (patch)", extra={"user_id": user_id, "task_id": task_id})
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
    task_out = TaskOut.model_validate(row).model_dump(mode="json")
    cache_set_json(make_task_key(user_id, task_id), task_out)
    cache_delete(make_task_list_key(user_id))
    enqueue_task_event("task_patched", user_id, task_id, {"title": row.title, "status": row.status.value})
    logger.info("task patched", extra={"user_id": user_id, "task_id": task_id})
    return task_out

@router.delete("/{task_id}", status_code=status.HTTP_204_NO_CONTENT)
def delete_task(task_id: int, db: Session = Depends(get_db), user_id: int = Depends(get_current_user_id)):
    row = db.query(Task).filter(Task.id == task_id, Task.user_id == user_id).first()
    if not row:
        logger.warning("task not found (delete)", extra={"user_id": user_id, "task_id": task_id})
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail="Task not found")
    db.delete(row)
    db.commit()
    cache_delete(make_task_key(user_id, task_id), make_task_list_key(user_id))
    enqueue_task_event("task_deleted", user_id, task_id, {})
    logger.info("task deleted", extra={"user_id": user_id, "task_id": task_id})
    return None