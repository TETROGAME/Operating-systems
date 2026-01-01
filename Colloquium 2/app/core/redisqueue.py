import redis
from rq import Queue

from app.core.config import RQ_REDIS_URL

redis_conn = redis.from_url(RQ_REDIS_URL)
default_queue = Queue("default", connection=redis_conn)