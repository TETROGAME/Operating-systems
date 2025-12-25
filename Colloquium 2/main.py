import uvicorn
from fastapi import FastAPI
from contextlib import asynccontextmanager

from app.routers.tasks import router as tasks_router
from app.storage.db import engine, Base

@asynccontextmanager
async def lifespan(app: FastAPI):
    Base.metadata.create_all(bind=engine)
    yield

app = FastAPI(
    title="To-Do List API",
    version="0.1.0",
    lifespan=lifespan,
)

app.include_router(tasks_router)

if __name__ == '__main__':
    uvicorn.run('main:app', reload=True)