import time
from typing import Callable, Awaitable

from fastapi import FastAPI, Request, Response
from prometheus_client import Counter, Histogram, generate_latest, CONTENT_TYPE_LATEST

HTTP_REQUESTS_TOTAL = Counter(
    "http_requests_total",
    "Total HTTP requests",
    ["method", "path", "status"],
)
HTTP_REQUEST_DURATION_SECONDS = Histogram(
    "http_request_duration_seconds",
    "HTTP request latency in seconds",
    ["method", "path", "status"],
    buckets=(0.05, 0.1, 0.25, 0.5, 1, 2, 5),
)

JOBS_PROCESSED_TOTAL = Counter(
    "jobs_processed_total",
    "Total background jobs processed",
    ["status"],
)
JOB_DURATION_SECONDS = Histogram(
    "job_duration_seconds",
    "Background job duration in seconds",
    buckets=(0.05, 0.1, 0.25, 0.5, 1, 2, 5, 10, 30),
)

def _normalize_path(path: str) -> str:
    return path

def add_metrics(app: FastAPI) -> None:
    @app.middleware("http")
    async def metrics_middleware(request: Request, call_next: Callable[[Request], Awaitable[Response]]):
        start = time.perf_counter()
        response: Response | None = None
        try:
            response = await call_next(request)
            return response
        finally:
            status = response.status_code if response else 500
            elapsed = time.perf_counter() - start
            path_label = _normalize_path(request.url.path)
            method = request.method

            HTTP_REQUESTS_TOTAL.labels(method=method, path=path_label, status=status).inc()
            HTTP_REQUEST_DURATION_SECONDS.labels(
                method=method, path=path_label, status=status
            ).observe(elapsed)

    @app.get("/metrics")
    def metrics() -> Response:
        return Response(generate_latest(), media_type=CONTENT_TYPE_LATEST)