import os
import time
import uuid
import pytest
import requests

BASE_URL = os.getenv("API_BASE_URL", "http://localhost:8000")
HEALTH_PATH = os.getenv("API_HEALTH_PATH", "/health")

def _ready():
    try:
        r = requests.get(BASE_URL + HEALTH_PATH, timeout=2)
        return r.status_code < 500
    except Exception:
        return False

@pytest.fixture(scope="session", autouse=True)
def wait_for_service():
    deadline = time.time() + 15
    while time.time() < deadline:
        if _ready():
            return
        time.sleep(1)
    pytest.skip(f"Service at {BASE_URL} is not ready")

def _register(username: str, password: str = "Passw0rd!"):
    return requests.post(f"{BASE_URL}/auth/register_user", json={"username": username, "password": password}, timeout=5)

def _login(username: str, password: str = "Passw0rd!"):
    return requests.post(f"{BASE_URL}/auth/login", json={"username": username, "password": password}, timeout=5)

def _auth_headers(token: str):
    return {"Authorization": f"Bearer {token}"}

def test_register_login_me():
    username = f"user-{uuid.uuid4()}"
    reg = _register(username)
    assert reg.status_code == 200, reg.text

    login = _login(username)
    assert login.status_code == 200, login.text
    token = login.json()["access_token"]

    me = requests.get(f"{BASE_URL}/auth/me", headers=_auth_headers(token), timeout=5)
    assert me.status_code == 200
    assert me.json()["username"] == username

def test_login_invalid_credentials():
    username = f"user-{uuid.uuid4()}"
    _register(username)
    bad = _login(username, "wrong")
    assert bad.status_code == 401
    assert bad.json()["detail"] == "Invalid credentials"

def test_tasks_crud_flow():
    username = f"user-{uuid.uuid4()}"
    _register(username)
    token = _login(username).json()["access_token"]
    headers = _auth_headers(token)

    create = requests.post(
        f"{BASE_URL}/tasks",
        json={"title": "Buy milk", "description": "3.2%", "status": "todo"},
        headers=headers,
        timeout=5,
    )
    assert create.status_code == 201, create.text
    task_id = create.json()["id"]

    lst = requests.get(f"{BASE_URL}/tasks", headers=headers, timeout=5)
    assert lst.status_code == 200
    assert any(t["id"] == task_id for t in lst.json())

    put = requests.put(
        f"{BASE_URL}/tasks/{task_id}",
        json={"title": "Buy milk and bread", "description": "whole grain", "status": "in_progress"},
        headers=headers,
        timeout=5,
    )
    assert put.status_code == 200
    assert put.json()["status"] == "in_progress"

    patch = requests.patch(
        f"{BASE_URL}/tasks/{task_id}",
        json={"status": "done"},
        headers=headers,
        timeout=5,
    )
    assert patch.status_code == 200
    assert patch.json()["status"] == "done"

    delete = requests.delete(f"{BASE_URL}/tasks/{task_id}", headers=headers, timeout=5)
    assert delete.status_code == 204

    after = requests.get(f"{BASE_URL}/tasks", headers=headers, timeout=5)
    assert after.status_code == 200
    assert all(t["id"] != task_id for t in after.json())

def test_unauthorized_access():
    resp = requests.get(f"{BASE_URL}/tasks", timeout=5)
    assert resp.status_code == 401