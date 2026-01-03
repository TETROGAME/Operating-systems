from fastapi import APIRouter
import os

router = APIRouter(prefix="/health", tags=["health"])
@router.get("/health")
def health():
    return {"status": "ok", "instance": os.getenv("HOSTNAME", "local")}