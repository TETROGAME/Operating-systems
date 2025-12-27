from fastapi import FastAPI, Request
from fastapi.responses import JSONResponse

from authx.exceptions import MissingTokenError, InvalidToken, JWTDecodeError


def install_exception_handlers(app: FastAPI) -> None:
    @app.exception_handler(MissingTokenError)
    async def missing_token_handler(request: Request, exc: MissingTokenError):
        return JSONResponse(status_code=401, content={"detail": "Missing 'Bearer' in 'Authorization' header."})

    @app.exception_handler(InvalidToken)
    async def invalid_token_handler(request: Request, exc: InvalidToken):
        return JSONResponse(status_code=401, content={"detail": "Invalid or expired token."})

    @app.exception_handler(JWTDecodeError)
    async def jwt_decode_error_handler(request: Request, exc: JWTDecodeError):
        # Example: "Signature has expired" -> 401
        return JSONResponse(status_code=401, content={"detail": str(exc) or "Token decode error"})