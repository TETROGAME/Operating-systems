import logging
import time
from typing import Any, Dict

from rq.job import Job

from app.core.redisqueue import default_queue
from app.core.metrics import JOBS_PROCESSED_TOTAL, JOB_DURATION_SECONDS

logger = logging.getLogger(__name__)

def handle_task_event(event: str, user_id: int, task_id: int | None, payload: Dict[str, Any]) -> None:
    started = time.perf_counter()
    try:
        logger.info(
            "task event",
            extra={"event": event, "user_id": user_id, "task_id": task_id, "payload": payload},
        )
    except Exception:
        logger.exception("task event failed", extra={"event": event, "user_id": user_id, "task_id": task_id})
        JOBS_PROCESSED_TOTAL.labels(status="error").inc()
        JOB_DURATION_SECONDS.observe(time.perf_counter() - started)
        raise
    else:
        JOBS_PROCESSED_TOTAL.labels(status="success").inc()
        JOB_DURATION_SECONDS.observe(time.perf_counter() - started)

def enqueue_task_event(event: str, user_id: int, task_id: int | None, payload: Dict[str, Any]) -> Job:
    return default_queue.enqueue(handle_task_event, event, user_id, task_id, payload)