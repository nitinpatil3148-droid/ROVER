import streamlit as st
import requests

# =====================================================
# PAGE CONFIGURATION
# =====================================================

st.set_page_config(
    page_title="Fire Fighting Rover",
    page_icon="🚒",
    layout="centered"
)

# =====================================================
# BACKEND URL
# =====================================================

BACKEND_URL = "https://YOUR-BACKEND-NAME.onrender.com"


# =====================================================
# SEND COMMAND
# =====================================================

def send_command(command):

    try:

        response = requests.post(
            f"{BACKEND_URL}/command/{command}",
            timeout=5
        )

        data = response.json()

        if data.get("success"):

            st.success(
                f"Command sent: {command}"
            )

        else:

            st.error(
                data.get(
                    "message",
                    "Command failed"
                )
            )

    except Exception as e:

        st.error(
            f"Backend connection error: {e}"
        )


# =====================================================
# TITLE
# =====================================================

st.title("🚒 Fire Fighting Rover")

st.write(
    "ESP32 2-Motor Rover Control System"
)

st.divider()


# =====================================================
# CONNECTION
# =====================================================

if st.button(
    "🔌 Test Backend",
    use_container_width=True
):

    try:

        response = requests.get(
            BACKEND_URL,
            timeout=5
        )

        data = response.json()

        st.success(
            data.get(
                "message",
                "Backend online"
            )
        )

    except Exception as e:

        st.error(
            f"Backend unavailable: {e}"
        )


st.divider()


# =====================================================
# MOVEMENT CONTROL
# =====================================================

st.subheader("Rover Control")


# Forward
if st.button(
    "⬆️ FORWARD",
    use_container_width=True
):

    send_command("F")


# Left / Stop / Right
col1, col2, col3 = st.columns(3)


with col1:

    if st.button(
        "⬅️ LEFT",
        use_container_width=True
    ):

        send_command("L")


with col2:

    if st.button(
        "⏹️ STOP",
        use_container_width=True
    ):

        send_command("S")


with col3:

    if st.button(
        "➡️ RIGHT",
        use_container_width=True
    ):

        send_command("R")


# Backward
if st.button(
    "⬇️ BACKWARD",
    use_container_width=True
):

    send_command("B")


st.divider()


st.info(
    "F = Forward | B = Backward | "
    "L = Left | R = Right | S = Stop"
)
