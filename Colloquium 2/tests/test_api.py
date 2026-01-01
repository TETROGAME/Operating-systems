import os
import pathlib
import uuid

import pytest
from fastapi.testclient import TestClient
from sqlalchemy.orm import close_all_sessions


os.environ.setdefault("DATABASE_URL", "sqlite:///./test_app.db")
os.environ.setdefault("JWT_SECRET_KEY", "test_secret_key")
os.environ.setdefault("JWT_ALGORITHM", "HS256")
os.environ.setdefault("JWT_ACCESS_TOKEN_EXPIRES_IN_MINUTES", "5")

from app.core.db import Base, engine  
from app.core.factory import create_app  

TEST_DB_PATH = pathlib.Path("test_app.db")


@pytest.fixture(autouse=True)
def setup_db():

    Base.metadata.drop_all(bind=engine)
    Base.metadata.create_all(bind=engine)
    yield

    close_all_sessions()
    Base.metadata.drop_all(bind=engine)
    engine.dispose()
    if TEST_DB_PATH.exists():
        TEST_DB_PATH.unlink()


@pytest.fixture
def client():
    app = create_app()
    with TestClient(app) as c:
        yield c


def _register(client: TestClient, username: str, password: str = "Passw0rd!") -> dict:
    resp = client.post("/auth/register", json={"username": username, "password": password})
    assert resp.status_code == 200, resp.text
    return resp.json()


def _login(client: TestClient, username: str, password: str = "Passw0rd!") -> str:
    resp = client.post("/auth/login", json={"username": username, "password": password})
    assert resp.status_code == 200, resp.text
    return resp.json()["access_token"]


def _auth_headers(token: str) -> dict:
    return {"Authorization": f"Bearer {token}"}


def test_register_and_login_success(client: TestClient):
    username = f"user-{uuid.uuid4()}"
    _register(client, username)
    token = _login(client, username)
    assert token

    me = client.get("/auth/me", headers=_auth_headers(token))
    assert me.status_code == 200
    body = me.json()
    assert body["username"] == username
    assert body["disabled"] is False


def test_register_duplicate_username(client: TestClient):
    username = f"user-{uuid.uuid4()}"
    _register(client, username)
    resp = client.post("/auth/register", json={"username": username, "password": "Passw0rd!"})
    assert resp.status_code == 400
    assert resp.json()["detail"] == "User already exists"


def test_login_invalid_credentials(client: TestClient):
    username = f"user-{uuid.uuid4()}"
    _register(client, username)
    resp = client.post("/auth/login", json={"username": username, "password": "wrong"})
    assert resp.status_code == 401
    assert resp.json()["detail"] == "Invalid credentials"


def test_me_requires_token(client: TestClient):
    resp = client.get("/auth/me")
    assert resp.status_code == 401
    assert "detail" in resp.json()


def test_me_rejects_invalid_token(client: TestClient):
    resp = client.get("/auth/me", headers=_auth_headers("invalid.token.value"))
    assert resp.status_code == 401
    assert "detail" in resp.json()


def test_tasks_crud_flow(client: TestClient):
    username = f"user-{uuid.uuid4()}"
    _register(client, username)
    token = _login(client, username)
    headers = _auth_headers(token)


    create_resp = client.post(
        "/tasks",
        json={"title": "Buy milk", "description": "3.2%", "status": "todo"},
        headers=headers,
    )
    assert create_resp.status_code == 201
    created = create_resp.json()
    task_id = created["id"]
    assert created["title"] == "Buy milk"
    assert created["status"] == "todo"


    list_resp = client.get("/tasks", headers=headers)
    assert list_resp.status_code == 200
    assert len(list_resp.json()) == 1


    get_resp = client.get(f"/tasks/{task_id}", headers=headers)
    assert get_resp.status_code == 200
    assert get_resp.json()["id"] == task_id


    put_resp = client.put(
        f"/tasks/{task_id}",
        json={"title": "Buy milk and bread", "description": "whole grain", "status": "in_progress"},
        headers=headers,
    )
    assert put_resp.status_code == 200
    assert put_resp.json()["status"] == "in_progress"


    patch_resp = client.patch(
        f"/tasks/{task_id}",
        json={"status": "done"},
        headers=headers,
    )
    assert patch_resp.status_code == 200
    assert patch_resp.json()["status"] == "done"


    del_resp = client.delete(f"/tasks/{task_id}", headers=headers)
    assert del_resp.status_code == 204


    list_after = client.get("/tasks", headers=headers)
    assert list_after.status_code == 200
    assert list_after.json() == []


def test_get_task_not_found(client: TestClient):
    username = f"user-{uuid.uuid4()}"
    _register(client, username)
    token = _login(client, username)
    headers = _auth_headers(token)

    resp = client.get("/tasks/9999", headers=headers)
    assert resp.status_code == 404
    assert resp.json()["detail"] == "Task not found"


def test_cannot_access_other_users_task(client: TestClient):

    user_a = f"user-{uuid.uuid4()}"
    _register(client, user_a)
    token_a = _login(client, user_a)
    headers_a = _auth_headers(token_a)

    create_resp = client.post(
        "/tasks",
        json={"title": "A's task", "description": None, "status": "todo"},
        headers=headers_a,
    )
    assert create_resp.status_code == 201
    task_id = create_resp.json()["id"]


    user_b = f"user-{uuid.uuid4()}"
    _register(client, user_b)
    token_b = _login(client, user_b)
    headers_b = _auth_headers(token_b)

    resp = client.get(f"/tasks/{task_id}", headers=headers_b)
    assert resp.status_code == 404
    assert resp.json()["detail"] == "Task not found"


def test_tasks_requires_auth(client: TestClient):
    resp = client.get("/tasks")
    assert resp.status_code == 401
    assert "detail" in resp.json()