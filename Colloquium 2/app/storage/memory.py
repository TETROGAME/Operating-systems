from typing import Dict, List, Optional
from datetime import datetime, timezone

from app.schemas import TaskCreate, TaskUpdate, TaskOut

class MemoryStorage:
    def __init__(self) -> None:
        self._tasks: Dict[int, TaskOut] = {}
        self._next_id: int = 1

    def _get_next_id(self) -> int:
        nid = self._next_id
        self._next_id += 1
        return nid

    def list(self) -> List[TaskOut]:
        return list(self._tasks.values())

    def create(self, data: TaskCreate) -> TaskOut:
        now = datetime.now(timezone.utc)
        new_task = TaskOut(
            id=self._get_next_id(),
            created_at=now,
            updated_at=now,
            **data.model_dump()
        )
        self._tasks[new_task.id] = new_task
        return new_task

    def get(self, task_id: int) -> Optional[TaskOut]:
        return self._tasks.get(task_id)

    def replace(self, task_id: int, data: TaskCreate) -> Optional[TaskOut]:
        existing = self._tasks.get(task_id)
        if not existing:
            return None
        now = datetime.now(timezone.utc)
        updated = TaskOut(
            id=task_id,
            created_at=existing.created_at,
            updated_at=now,
            **data.model_dump()
        )
        self._tasks[task_id] = updated
        return updated

    def update(self, task_id: int, data: TaskUpdate) -> Optional[TaskOut]:
        existing = self._tasks.get(task_id)
        if not existing:
            return None
        payload = existing.model_dump()
        for field, value in data.model_dump(exclude_unset=True).items():
            payload[field] = value
        payload["updated_at"] = datetime.now(timezone.utc)
        updated = TaskOut(**payload)
        self._tasks[task_id] = updated
        return updated

    def delete(self, task_id: int) -> bool:
        if task_id in self._tasks:
            del self._tasks[task_id]
            return True
        return False

storage = MemoryStorage()