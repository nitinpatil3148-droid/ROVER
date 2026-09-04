#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

// =====================================================
// WIFI DETAILS
// =====================================================

const char* WIFI_SSID = "VIRUS";
const char* WIFI_PASSWORD = "00000000";

// =====================================================
// RENDER BACKEND DETAILS
// =====================================================

// Example:
// your-app-name.onrender.com

const char* SERVER_HOST = "your-app-name.onrender.com";
const int SERVER_PORT = 443;
const char* SERVER_PATH = "/ws/esp32";

// =====================================================
// RELAY PINS
// =====================================================

// MOTOR 1
#define M1_R1 5
#define M1_R2 18

// MOTOR 2
#define M2_R1 19
#define M2_R2 21

// Relay module is assumed ACTIVE LOW
#define RELAY_ON LOW
#define RELAY_OFF HIGH

// =====================================================
// WEBSOCKET
// =====================================================

WebSocketsClient webSocket;

// =====================================================
// MOTOR FUNCTIONS
// =====================================================

void motor1Forward() {
  digitalWrite(M1_R1, RELAY_ON);
  digitalWrite(M1_R2, RELAY_OFF);
}

void motor1Backward() {
  digitalWrite(M1_R1, RELAY_OFF);
  digitalWrite(M1_R2, RELAY_ON);
}

void motor1Stop() {
  digitalWrite(M1_R1, RELAY_OFF);
  digitalWrite(M1_R2, RELAY_OFF);
}


void motor2Forward() {
  digitalWrite(M2_R1, RELAY_ON);
  digitalWrite(M2_R2, RELAY_OFF);
}

void motor2Backward() {
  digitalWrite(M2_R1, RELAY_OFF);
  digitalWrite(M2_R2, RELAY_ON);
}

void motor2Stop() {
  digitalWrite(M2_R1, RELAY_OFF);
  digitalWrite(M2_R2, RELAY_OFF);
}

// =====================================================
// ROVER MOVEMENT
// =====================================================

void forward() {

  motor1Forward();
  motor2Forward();

  Serial.println("ROVER → FORWARD");
}

void backward() {

  motor1Backward();
  motor2Backward();

  Serial.println("ROVER → BACKWARD");
}

void left() {

  motor1Backward();
  motor2Forward();

  Serial.println("ROVER → LEFT");
}

void right() {

  motor1Forward();
  motor2Backward();

  Serial.println("ROVER → RIGHT");
}

void stopRover() {

  motor1Stop();
  motor2Stop();

  Serial.println("ROVER → STOP");
}

// =====================================================
// COMMAND PROCESSING
// =====================================================

void processCommand(String command) {

  command.trim();
  command.toUpperCase();

  Serial.print("Received command: ");
  Serial.println(command);

  if (command == "F") {

    forward();

  } else if (command == "B") {

    backward();

  } else if (command == "L") {

    left();

  } else if (command == "R") {

    right();

  } else if (command == "S") {

    stopRover();

  } else {

    Serial.println("Unknown command");

    stopRover();
  }
}

// =====================================================
// WEBSOCKET EVENT
// =====================================================

void webSocketEvent(
  WStype_t type,
  uint8_t* payload,
  size_t length
) {

  switch (type) {

    case WStype_DISCONNECTED:

      Serial.println("WebSocket disconnected");

      stopRover();

      break;


    case WStype_CONNECTED:

      Serial.println("WebSocket connected to backend");

      // Tell backend that ESP32 is ready
      webSocket.sendTXT("ESP32_READY");

      break;


    case WStype_TEXT: {

      String command = "";

      for (size_t i = 0; i < length; i++) {
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

void connectWiFi() {

  Serial.println();
  Serial.println("Connecting to Wi-Fi...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();

  Serial.println("Wi-Fi connected");

  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // Relay pins
  pinMode(M1_R1, OUTPUT);
  pinMode(M1_R2, OUTPUT);

  pinMode(M2_R1, OUTPUT);
  pinMode(M2_R2, OUTPUT);

  // IMPORTANT:
  // Start with motors stopped
  stopRover();

  Serial.println();
  Serial.println("================================");
  Serial.println("   ESP32 FIRE FIGHTING ROVER");
  Serial.println("================================");

  connectWiFi();

  // Secure WebSocket
  webSocket.beginSSL(
    SERVER_HOST,
    SERVER_PORT,
    SERVER_PATH
  );

  webSocket.onEvent(webSocketEvent);

  webSocket.setReconnectInterval(5000);

  Serial.println("Connecting to backend...");
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  webSocket.loop();
}
