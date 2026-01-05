import logging
from contextlib import asynccontextmanager
from fastapi import FastAPI
from starlette.middleware.cors import CORSMiddleware

from app.core.db import Base, engine
from app.core.errors import install_exception_handlers
from app.core.openapi import install_openapi_security
from app.core.logging_config import configure_logging
from app.core.metrics import add_metrics
from app.routers.auth import router as auth_router
from app.routers.tasks import router as tasks_router
from app.routers.health import router as health_router

logger = logging.getLogger(__name__)

@asynccontextmanager
async def lifespan(app: FastAPI):
    Base.metadata.create_all(bind=engine)
    yield

def create_app() -> FastAPI:
    configure_logging(level="INFO")
    app = FastAPI(
        title="To-Do List API",
        version="1.0.0",
        lifespan=lifespan,
    )

    install_exception_handlers(app)
    install_openapi_security(app)
    add_metrics(app)

    app.include_router(auth_router)
    app.include_router(tasks_router)
    app.include_router(health_router)

    app.add_middleware(
        CORSMiddleware,
        allow_origins=["*"],
        allow_credentials=False,
        allow_methods=["*"],
        allow_headers=["*"],
    )

    logger.info("Application started")
    return app