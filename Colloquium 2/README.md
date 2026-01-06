# To-Do List API

A JWT-protected REST API for managing tasks built with FastAPI, SQLAlchemy, and Redis-backed background jobs. Includes a lightweight frontend, metrics, and Dockerized deployment.

---

## Features
- Auth: user/admin registration, JWT login (AuthX), protected endpoints.
- Tasks: per-user CRUD (create/list/get/update/patch/delete) with status (`todo`, `in_progress`, `done`).
- Data: SQLAlchemy ORM (SQLite by default; can switch to Postgres).
- Caching: Redis cache for task lists/items.
- Background jobs: RQ queue (Redis) for task events.
- Observability: Prometheus metrics (`/metrics`), structured logging to rotating file.
- API docs: Swagger UI at `/docs`, ReDoc at `/redoc`.
- Frontend: simple static UI (`front/`) for auth and tasks.
- Health check: `/health`.

---

## Tech Stack
- **Python 3.11+**, **FastAPI**, **Uvicorn**
- **Pydantic v2**, **SQLAlchemy 2**
- **AuthX** + **passlib[bcrypt]** for JWT auth & password hashing
- **Redis** for cache + **RQ** for background jobs
- **Prometheus client** for metrics
- **Nginx** as reverse proxy (rate limiting, fanout)
- **Docker / docker-compose** for multi-service setup
- Testing: **pytest**, **requests**, **fastapi.testclient**

---

## Project Structure
```
Colloquium 2/
├─ app/
│  ├─ core/              # config, DB, logging, metrics, OpenAPI, Redis queue
│  ├─ models/            # ORM models (User, Task)
│  ├─ routers/           # FastAPI routers (auth, tasks, health)
│  ├─ schemas/           # Pydantic schemas (auth, task)
│  ├─ security/          # Auth helpers (AuthX setup, deps)
│  └─ tasks/             # Background jobs (RQ handlers)
├─ front/                # Static frontend (index.html, main.js, style.css)
├─ deploy/               # Dockerfile, docker-compose, nginx.conf
├─ tests/                # Pytest suites + HTTP scratch file
├─ main.py               # App entrypoint
├─ requirements.txt
└─ README.md
```

---

## Quickstart (Local)

1) **Install deps**
```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

2) **Set environment** (create `.env` or export vars):
```
DATABASE_URL=sqlite:///./data/app.db
JWT_SECRET_KEY=CHANGE_ME
JWT_ALGORITHM=HS256
JWT_ACCESS_TOKEN_EXPIRES_IN_MINUTES=15
REDIS_URL=redis://localhost:6379/0
RQ_REDIS_URL=redis://localhost:6379/3
CACHE_TASK_TTL=300
```
> Note: app will raise if `JWT_SECRET_KEY` is weak/missing.

3) **Run API**
```bash
uvicorn main:app --reload
```
- Swagger UI: http://localhost:8000/docs  
- Metrics: http://localhost:8000/metrics  
- Health: http://localhost:8000/health

4) **Run frontend**
Serve `front/` statically (e.g., `python -m http.server` inside `front/`) and point it to the API (`http://localhost:8000` by default, configurable via `localStorage.api_base`).

---

## Quickstart (Docker Compose)

From the repo root (note `deploy/` paths in compose):

```bash
cd deploy
docker compose up --build
```

Services:
- `app1`, `app2`: FastAPI app instances (uvicorn)
- `rq-worker`: RQ worker for background jobs
- `redis`: Redis backend
- `nginx`: reverse proxy on host port **8000** (rate limiting enabled)

Env is loaded from `../.env`. Volumes `../data` and `../logs` are mounted for persistence.

---

## Testing

Local tests (uses SQLite and patched Redis):
```bash
pytest
```

Dockerized API smoke tests (expects running service; set `API_BASE_URL`/`API_HEALTH_PATH` if needed):
```bash
pytest tests/test_api_docker.py
```

HTTP scratch requests:
```
tests/test_main.http
```
(use an HTTP client like VS Code REST Client; replace tokens/placeholders).

---

## API Overview

- `POST /auth/register_user` / `POST /auth/register_admin`
- `POST /auth/login`
- `GET /auth/me`
- `GET /tasks`
- `POST /tasks`
- `GET /tasks/{id}`
- `PUT /tasks/{id}`
- `PATCH /tasks/{id}`
- `DELETE /tasks/{id}`
- `GET /health`
- `GET /metrics` (Prometheus)

All `/tasks` endpoints require `Authorization: Bearer <JWT>`.