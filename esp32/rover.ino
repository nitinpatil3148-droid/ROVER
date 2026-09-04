#include <WiFi.h>
#include <WebSocketsClient.h>

// =====================================================
// WIFI
// =====================================================

const char* WIFI_SSID = "mallappa patil";
const char* WIFI_PASSWORD = "abc123xyz.";


// =====================================================
// RENDER BACKEND
// =====================================================

const char* SERVER_HOST = "rover-okmg.onrender.com";

const int SERVER_PORT = 443;

const char* SERVER_PATH = "/ws/esp32";


// =====================================================
// MOTOR PINS
// Based on your existing working code
// =====================================================

// LEFT MOTOR
#define LmotorA 27
#define LmotorB 26

// RIGHT MOTOR
#define RmotorA 25
#define RmotorB 33


// =====================================================
// WEBSOCKET
// =====================================================

WebSocketsClient webSocket;


// =====================================================
// FORWARD
// =====================================================

void Forward()
{
    digitalWrite(LmotorA, HIGH);
    digitalWrite(LmotorB, LOW);

    digitalWrite(RmotorA, HIGH);
    digitalWrite(RmotorB, LOW);

    Serial.println("FORWARD");
}


// =====================================================
// BACKWARD
// =====================================================

void Backward()
{
    digitalWrite(LmotorA, LOW);
    digitalWrite(LmotorB, HIGH);

    digitalWrite(RmotorA, LOW);
    digitalWrite(RmotorB, HIGH);

    Serial.println("BACKWARD");
}


// =====================================================
// RIGHT
// =====================================================

void Right()
{
    digitalWrite(LmotorA, HIGH);
    digitalWrite(LmotorB, LOW);

    digitalWrite(RmotorA, LOW);
    digitalWrite(RmotorB, HIGH);

    Serial.println("RIGHT");
}


// =====================================================
// LEFT
// =====================================================

void Left()
{
    digitalWrite(LmotorA, LOW);
    digitalWrite(LmotorB, HIGH);

    digitalWrite(RmotorA, HIGH);
    digitalWrite(RmotorB, LOW);

    Serial.println("LEFT");
}


// =====================================================
// STOP
// =====================================================

void Stop()
{
    digitalWrite(LmotorA, LOW);
    digitalWrite(LmotorB, LOW);

    digitalWrite(RmotorA, LOW);
    digitalWrite(RmotorB, LOW);

    Serial.println("STOP");
}


// =====================================================
// PROCESS COMMAND
// =====================================================

void processCommand(String command)
{
    command.trim();
    command.toUpperCase();

    Serial.print("Command: ");
    Serial.println(command);

    if (command == "F")
    {
        Forward();
    }
    else if (command == "B")
    {
        Backward();
    }
    else if (command == "L")
    {
        Left();
    }
    else if (command == "R")
    {
        Right();
    }
    else if (command == "S")
    {
        Stop();
    }
    else
    {
        Stop();
    }
}


// =====================================================
// WEBSOCKET EVENT
// =====================================================

void webSocketEvent(
    WStype_t type,
    uint8_t* payload,
    size_t length
)
{
    switch(type)
    {

        // ---------------------------------------------
        // DISCONNECTED
        // ---------------------------------------------

        case WStype_DISCONNECTED:

            Serial.println("Backend disconnected");

            // SAFETY
            Stop();

            break;


        // ---------------------------------------------
        // CONNECTED
        // ---------------------------------------------

        case WStype_CONNECTED:

            Serial.println("Backend connected");

            webSocket.sendTXT("ESP32_READY");

            break;


        // ---------------------------------------------
        // COMMAND RECEIVED
        // ---------------------------------------------

        case WStype_TEXT:
        {
            String command = "";

            for(size_t i = 0; i < length; i++)
            {
                command += (char)payload[i];
            }

            processCommand(command);

            break;
        }


        default:
            break;
    }
}


// =====================================================
// WIFI CONNECTION
// =====================================================

void connectWiFi()
{
    Serial.println();
    Serial.println("Connecting to Wi-Fi...");

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    Serial.println("Wi-Fi Connected");

    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
}


// =====================================================
// SETUP
// =====================================================

void setup()
{
    Serial.begin(115200);


    // LEFT MOTOR
    pinMode(LmotorA, OUTPUT);
    pinMode(LmotorB, OUTPUT);


    // RIGHT MOTOR
    pinMode(RmotorA, OUTPUT);
    pinMode(RmotorB, OUTPUT);


    // SAFETY
    Stop();


    Serial.println();
    Serial.println("================================");
    Serial.println("     FIRE FIGHTING ROVER");
    Serial.println("       2 MOTOR VERSION");
    Serial.println("================================");


    // CONNECT WIFI
    connectWiFi();


    // CONNECT TO RENDER
    webSocket.beginSSL(
        SERVER_HOST,
        SERVER_PORT,
        SERVER_PATH
    );


    webSocket.onEvent(
        webSocketEvent
    );


    webSocket.setReconnectInterval(5000);


    Serial.println("Connecting to backend...");
}


// =====================================================
// LOOP
// =====================================================

void loop()
{
    webSocket.loop();
}
