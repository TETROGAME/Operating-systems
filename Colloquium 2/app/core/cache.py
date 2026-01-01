import json
from typing import Any, Callable

import redis
from redis.exceptions import RedisError

from app.core.config import REDIS_URL, CACHE_TASK_TTL

redis_client = redis.Redis.from_url(REDIS_URL, decode_responses=True)

def _safe_call(fn: Callable[[], Any], default=None):
    try:
        return fn()
    except RedisError:
        return default

def cache_get_json(key: str):
    def _fn():
        raw = redis_client.get(key)
        if raw is None:
            return None
        return json.loads(raw)
    return _safe_call(_fn, default=None)

def cache_set_json(key: str, value: Any, ttl: int | None = None):
    ttl = ttl or CACHE_TASK_TTL
    return _safe_call(lambda: redis_client.setex(key, ttl, json.dumps(value)), default=None)

def cache_delete(*keys: str):
    if not keys:
        return None
    return _safe_call(lambda: redis_client.delete(*keys), default=None)

def make_task_list_key(user_id: int) -> str:
    return f"user:{user_id}:tasks"

def make_task_key(user_id: int, task_id: int) -> str:
    return f"user:{user_id}:task:{task_id}"