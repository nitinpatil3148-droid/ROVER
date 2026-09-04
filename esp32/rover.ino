
#include <WiFi.h>

// ==============================
// MOTOR 1 RELAYS
// ==============================
#define M1_R1 5
#define M1_R2 18

// ==============================
// MOTOR 2 RELAYS
// ==============================
#define M2_R1 19
#define M2_R2 21

// Relay logic
// Most relay modules are ACTIVE LOW
#define RELAY_ON  LOW
#define RELAY_OFF HIGH


void setup()
{
    Serial.begin(115200);

    // Motor 1
    pinMode(M1_R1, OUTPUT);
    pinMode(M1_R2, OUTPUT);

    // Motor 2
    pinMode(M2_R1, OUTPUT);
    pinMode(M2_R2, OUTPUT);

    // Safety: motors OFF
    stopRover();

    Serial.println("================================");
    Serial.println("   ESP32 2-MOTOR ROVER");
    Serial.println("================================");
    Serial.println("System Ready");
}


// ==============================
// MAIN LOOP
// ==============================

void loop()
{
    // Temporary testing
    // Later Render backend commands
    // will be received here.

}


// ==============================
// MOTOR 1
// ==============================

void motor1Forward()
{
    digitalWrite(M1_R1, RELAY_ON);
    digitalWrite(M1_R2, RELAY_OFF);
}

void motor1Backward()
{
    digitalWrite(M1_R1, RELAY_OFF);
    digitalWrite(M1_R2, RELAY_ON);
}

void motor1Stop()
{
    digitalWrite(M1_R1, RELAY_OFF);
    digitalWrite(M1_R2, RELAY_OFF);
}


// ==============================
// MOTOR 2
// ==============================

void motor2Forward()
{
    digitalWrite(M2_R1, RELAY_ON);
    digitalWrite(M2_R2, RELAY_OFF);
}

void motor2Backward()
{
    digitalWrite(M2_R1, RELAY_OFF);
    digitalWrite(M2_R2, RELAY_ON);
}

void motor2Stop()
{
    digitalWrite(M2_R1, RELAY_OFF);
    digitalWrite(M2_R2, RELAY_OFF);
}


// ==============================
// ROVER CONTROL
// ==============================

void forward()
{
    motor1Forward();
    motor2Forward();

    Serial.println("FORWARD");
}


void backward()
{
    motor1Backward();
    motor2Backward();

    Serial.println("BACKWARD");
}


void left()
{
    motor1Backward();
    motor2Forward();

    Serial.println("LEFT");
}


void right()
{
    motor1Forward();
    motor2Backward();

    Serial.println("RIGHT");
}


void stopRover()
{
    motor1Stop();
    motor2Stop();

    Serial.println("STOP");
}
