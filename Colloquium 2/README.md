# To-Do List API

A simple REST API for managing tasks (To‑Do List) built with Python and FastAPI.

This project currently uses in‑session (in‑memory) storage — data exists only while the app process is running. It’s ideal for learning and can be later replaced with a database (e.g., PostgreSQL).

---

## Features

- CRUD endpoints for tasks:
  - Create, Read (list and by id), Update (PUT/PATCH), Delete
- JSON request/response format
- Proper HTTP status codes (200, 201, 204, 404, 422)
- Clean code structure with routers, schemas, and storage layers
- In‑memory storage (resets on restart)

Used technologies:
- Python (3.12) with FastAPI
- Uvicorn (ASGI web server)

Planned:
- PostgreSQL with SQLAlchemy (future)
- Docker for deployment (future)
- Logging, metrics, auth/rate limiting (future)

---

## Project Structure

Inside the Colloquium 2/ directory:

```
Colloquium 2/
├─ main.py                     # App entrypoint (starts FastAPI)
└─ app/
   ├─ schemas.py           # Pydantic schemas (request/response models)
   ├─ routers/
   │  └─ tasks.py              # HTTP routes/endpoints for /tasks
   ├─ crud/
   │  └─ tasks.py              # Task operations (business layer)
   └─ storage/
      └─ memory.py             # In‑memory storage adapter
```

Design rationale:
- routers/: HTTP layer (FastAPI endpoints)
- crud/: application logic layer (calls storage)
- storage/: data access layer (currently in‑memory; will be swapped for DB)
- schemas.py: Pydantic models used by routers/crud/storage

---

## Data Model

Statuses: `"todo" | "in_progress" | "done"`

- TaskCreate (request for POST/PUT):
  - title: string (required, 1–200 chars)
  - description: string (optional)
  - status: one of the statuses above (default: `todo`)

- TaskUpdate (request for PATCH):
  - title: string (optional)
  - description: string (optional)
  - status: status (optional)

- TaskOut (response):
  - id: integer
  - title: string
  - description: string or null
  - status: status
  - created_at: datetime or null (not set in memory storage)
  - updated_at: datetime or null (not set in memory storage)

Note: With in‑memory storage, `created_at` and `updated_at` are currently not populated and will be `null`.

---

## API Reference

Base URL (local dev): `http://localhost:8000`

- GET `/tasks`
  - Returns a list of tasks.
  - 200 OK

- POST `/tasks`
  - Creates a new task.
  - Body (JSON):
    ```json
    {
      "title": "Buy groceries",
      "description": "Milk, bread, bananas",
      "status": "todo"
    }
    ```
  - 201 Created
  - Response (JSON):
    ```json
    {
      "id": 1,
      "title": "Buy groceries",
      "description": "Milk, bread, bananas",
      "status": "todo",
      "created_at": null,
      "updated_at": null
    }
    ```

- GET `/tasks/{id}`
  - Returns a task by ID.
  - 200 OK or 404 Not Found

- PUT `/tasks/{id}`
  - Replaces the entire task (send all fields like POST).
  - 200 OK or 404 Not Found

- PATCH `/tasks/{id}`
  - Partially updates a task (send only changed fields).
  - Example:
    ```json
    { "status": "done" }
    ```
  - 200 OK or 404 Not Found

- DELETE `/tasks/{id}`
  - Deletes a task by ID.
  - 204 No Content or 404 Not Found

Validation errors (e.g., wrong types or status) return 422 Unprocessable Entity.

---

## Running the app

From the Colloquium 2/ directory:

- Using uvicorn:
```bash
uvicorn main:app --reload
```

- Or via Python (starts uvicorn from code):
```bash
python main.py
```

Open the interactive docs:
- Swagger UI: http://localhost:8000/docs

---

## Usage Examples (curl)

Create a task:
```bash
curl -X POST http://localhost:8000/tasks \
  -H "Content-Type: application/json" \
  -d '{"title":"Buy milk","description":"3.2% fat","status":"todo"}'
```

List tasks:
```bash
curl http://localhost:8000/tasks
```

Get by ID:
```bash
curl http://localhost:8000/tasks/1
```

Patch (partial update):
```bash
curl -X PATCH http://localhost:8000/tasks/1 \
  -H "Content-Type: application/json" \
  -d '{"status":"done"}'
```

Put (replace):
```bash
curl -X PUT http://localhost:8000/tasks/1 \
  -H "Content-Type: application/json" \
  -d '{"title":"Buy milk and bread","description":"Grab change","status":"in_progress"}'
```

Delete:
```bash
curl -X DELETE http://localhost:8000/tasks/1
```

---


## Roadmap

- Storage:
  - Replace in‑memory with PostgreSQL
  - Add created_at/updated_at timestamps
- Observability:
  - Request logging, error logging 
- Platform:
  - API gateway/rate limiting (optional)
- Security:
  - JWT Auth 
  - Validation improvements


