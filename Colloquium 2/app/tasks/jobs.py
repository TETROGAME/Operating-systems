from datetime import datetime
from typing import Any, Dict

from rq.job import Job

from app.core.redisqueue import default_queue

def handle_task_event(event: str, user_id: int, task_id: int | None, payload: Dict[str, Any]) -> None:
    print(f"[{datetime.utcnow().isoformat()}] event={event} user={user_id} task={task_id} payload={payload}")

def enqueue_task_event(event: str, user_id: int, task_id: int | None, payload: Dict[str, Any]) -> Job:
    return default_queue.enqueue(handle_task_event, event, user_id, task_id, payload)