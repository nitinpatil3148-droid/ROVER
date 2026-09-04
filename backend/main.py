from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware
from typing import Optional

app = FastAPI(
    title="Fire Fighting Rover Backend",
    version="1.0"
)

# =====================================================
# CORS
# =====================================================

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# =====================================================
# ESP32 CONNECTION
# =====================================================

esp32_socket: Optional[WebSocket] = None


# =====================================================
# HOME
# =====================================================

@app.get("/")
def home():

    return {
        "status": "online",
        "message": "Fire Fighting Rover Backend"
    }


# =====================================================
# ESP32 WEBSOCKET
# =====================================================

@app.websocket("/ws/esp32")
async def esp32_websocket(websocket: WebSocket):

    global esp32_socket

    await websocket.accept()

    esp32_socket = websocket

    print("ESP32 connected")

    try:

        while True:

            message = await websocket.receive_text()

            print("ESP32:", message)

    except WebSocketDisconnect:

        print("ESP32 disconnected")

        esp32_socket = None

    except Exception as e:

        print("WebSocket error:", e)

        esp32_socket = None


# =====================================================
# SEND COMMAND TO ESP32
# =====================================================

@app.post("/command/{command}")
async def send_command(command: str):

    global esp32_socket

    command = command.upper()

    allowed_commands = [
        "F",
        "B",
        "L",
        "R",
        "S"
    ]

    if command not in allowed_commands:

        return {
            "success": False,
            "message": "Invalid command"
        }

    if esp32_socket is None:

        return {
            "success": False,
            "message": "ESP32 is not connected"
        }

    try:

        await esp32_socket.send_text(command)

        return {
            "success": True,
            "command": command
        }

    except Exception as e:

        esp32_socket = None

        return {
            "success": False,
            "message": str(e)
        }
