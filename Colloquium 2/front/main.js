const API_BASE = localStorage.getItem('api_base') || 'http://localhost:8000';

const qs = (id) => document.getElementById(id);
const msg = (text, isError = true) => {
  const el = qs('message');
  el.style.color = isError ? 'red' : 'green';
  el.textContent = text || '';
};

let token = localStorage.getItem('token') || '';

function setToken(t) {
  token = t;
  if (t) localStorage.setItem('token', t);
  else localStorage.removeItem('token');
}

async function api(path, opts = {}) {
  const headers = opts.headers || {};
  if (token) headers['Authorization'] = `Bearer ${token}`;
  const res = await fetch(API_BASE + path, { ...opts, headers });
  let data = null;
  try { data = await res.json(); } catch (_) { data = null; }
  if (!res.ok) {
    const detail = data?.detail || res.statusText;
    throw new Error(detail);
  }
  return data;
}

async function login(username, password) {
  return api('/auth/login', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ username, password })
  });
}
async function registerUser(username, password) {
  return api('/auth/register_user', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ username, password })
  });
}
async function registerAdmin(username, password) {
  return api('/auth/register_admin', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ username, password })
  });
}
async function me() { return api('/auth/me'); }
async function listTasks() { return api('/tasks'); }
async function createTask(payload) {
  return api('/tasks', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(payload)
  });
}
async function patchTask(id, payload) {
  return api(`/tasks/${id}`, {
    method: 'PATCH',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(payload)
  });
}
async function deleteTask(id) {
  return api(`/tasks/${id}`, { method: 'DELETE' });
}

function renderTasks(tasks) {
  const ul = qs('tasks');
  ul.innerHTML = '';
  if (!tasks.length) {
    ul.innerHTML = '<li>Задач нет</li>';
    return;
  }
  tasks.forEach((t) => {
    const li = document.createElement('li');
    li.className = 'task-item';
    li.innerHTML = `
      <div><strong>${t.title}</strong> [${t.status}]</div>
      <div>${t.description ?? ''}</div>
      <div class="task-actions">
        <select data-id="${t.id}" class="status-select">
          <option value="todo" ${t.status === 'todo' ? 'selected' : ''}>todo</option>
          <option value="in_progress" ${t.status === 'in_progress' ? 'selected' : ''}>in_progress</option>
          <option value="done" ${t.status === 'done' ? 'selected' : ''}>done</option>
        </select>
        <button data-del="${t.id}">Удалить</button>
      </div>
    `;
    ul.appendChild(li);
  });

  ul.querySelectorAll('.status-select').forEach((sel) => {
    sel.onchange = async (e) => {
      try {
        await patchTask(sel.dataset.id, { status: e.target.value });
        await loadTasks();
        msg('');
      } catch (err) { msg(err.message); }
    };
  });
  ul.querySelectorAll('button[data-del]').forEach((btn) => {
    btn.onclick = async () => {
      try {
        await deleteTask(btn.dataset.del);
        await loadTasks();
        msg('');
      } catch (err) { msg(err.message); }
    };
  });
}

async function loadProfile() {
  const user = await me();
  qs('user-info').textContent = `Пользователь: ${user.username}`;
}

async function loadTasks() {
  const tasks = await listTasks();
  renderTasks(tasks);
}

async function handleAuth(mode) {
  const username = qs('username').value.trim();
  const password = qs('password').value;
  if (!username || !password) return msg('Введите логин и пароль');
  try {
    msg('');
    if (mode === 'register_user') await registerUser(username, password);
    if (mode === 'register_admin') await registerAdmin(username, password);
    const { access_token } = await login(username, password);
    setToken(access_token);
    await loadProfile();
    await loadTasks();
    qs('auth-card').classList.add('hidden');
    qs('app-card').classList.remove('hidden');
  } catch (err) {
    msg(err.message);
  }
}

function init() {
  qs('auth-submit').onclick = () => handleAuth(currentMode);
  qs('toggle-mode').onclick = () => {
    currentMode = currentMode === 'login' ? 'register_user' : 'login';
    qs('auth-title').textContent = currentMode === 'login' ? 'Вход' : 'Регистрация';
    qs('auth-submit').textContent = currentMode === 'login' ? 'Войти' : 'Зарегистрироваться';
    qs('toggle-mode').textContent = currentMode === 'login' ? 'Создать аккаунт' : 'Есть аккаунт? Войти';
    msg('');
  };
  qs('register-admin').onclick = () => handleAuth('register_admin');
  qs('logout').onclick = () => {
    setToken('');
    qs('app-card').classList.add('hidden');
    qs('auth-card').classList.remove('hidden');
    msg('Вышли из системы', false);
  };
  qs('create-task').onclick = async () => {
    const title = qs('task-title').value.trim();
    const description = qs('task-desc').value.trim();
    const status = qs('task-status').value;
    if (!title) return msg('Название обязательно');
    try {
      await createTask({ title, description, status });
      qs('task-title').value = '';
      qs('task-desc').value = '';
      qs('task-status').value = 'todo';
      await loadTasks();
      msg('Создано', false);
    } catch (err) { msg(err.message); }
  };

  if (token) {
    loadProfile().then(loadTasks).then(() => {
      qs('auth-card').classList.add('hidden');
      qs('app-card').classList.remove('hidden');
    }).catch(() => {
      setToken('');
    });
  }
}

let currentMode = 'login';
init();