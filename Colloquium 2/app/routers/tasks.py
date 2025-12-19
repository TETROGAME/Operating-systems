from typing import List

from fastapi import APIRouter, status

from app.TaskSchemes import TaskCreate, TaskUpdate, TaskOut
from app.crud.tasks import (
    list_tasks,
    create_task,
    get_task,
    put_task,
    patch_task,
    delete_task,
)

router = APIRouter(prefix="/tasks", tags=["tasks"])


@router.get("", response_model=List[TaskOut], summary="Get all tasks")
async def list_tasks_endpoint():
    return list_tasks()


@router.post("", response_model=TaskOut, status_code=status.HTTP_201_CREATED, summary="Create new task")
async def create_task_endpoint(payload: TaskCreate):
    return create_task(payload)


@router.get("/{task_id}", response_model=TaskOut, summary="Get task via ID")
async def get_task_endpoint(task_id: int):
    return get_task(task_id)


@router.put("/{task_id}", response_model=TaskOut, summary="Update whole task via ID")
async def put_task_endpoint(task_id: int, payload: TaskCreate):
    return put_task(task_id, payload)


@router.patch("/{task_id}", response_model=TaskOut, summary="Update task partially via ID")
async def patch_task_endpoint(task_id: int, payload: TaskUpdate):
    return patch_task(task_id, payload)


@router.delete("/{task_id}", status_code=status.HTTP_204_NO_CONTENT, summary="Delete task via ID")
async def delete_task_endpoint(task_id: int):
    delete_task(task_id)
    return
