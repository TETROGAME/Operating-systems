from contextlib import asynccontextmanager
from fastapi import FastAPI

from app.core.db import Base, engine
from app.core.errors import install_exception_handlers
from app.core.openapi import install_openapi_security
from app.routers.auth import router as auth_router
from app.routers.tasks import router as tasks_router


@asynccontextmanager
async def lifespan(app: FastAPI):
    Base.metadata.create_all(bind=engine)
    yield


def create_app() -> FastAPI:
    app = FastAPI(
        title="To-Do List API",
        version="1.0.0",
        lifespan=lifespan,
    )

    install_exception_handlers(app)

    install_openapi_security(app)

    app.include_router(auth_router)
    app.include_router(tasks_router)
    return app