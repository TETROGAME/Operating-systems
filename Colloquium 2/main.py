import uvicorn
from fastapi import FastAPI
from app.routers.tasks import router as tasks_router

app = FastAPI(
    title="To-Do List API",
    version="0.1.0"
)

app.include_router(tasks_router)

if __name__ == '__main__':
    uvicorn.run('main:app', reload=True)