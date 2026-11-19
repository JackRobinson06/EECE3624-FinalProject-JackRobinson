#include <Wire.h>
#include <ZumoShield.h>
#include <SoftwareSerial.h>

ZumoBuzzer buzzer;
ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

SoftwareSerial bluetooth(2, 6);

#define QTR_THRESHOLD 1500
#define DRIVE_SPEED 250
#define TURN_SPEED 250
#define REVERSE_SPEED 300
#define REVERSE_DURATION 200
#define TURN_DURATION 300
#define TIMEOUT 300

unsigned int sensor_values[6];

char command = 'S';
unsigned long lastTime = 0;

void setup()
{
    Serial.begin(9600);
    bluetooth.begin(9600);
}

void loop()
{
    // Check if bluetooth received something
    if (bluetooth.available())
    {
        char input = bluetooth.read();

        if (input == 'U' || input == 'D' || input == 'L' ||
            input == 'R' || input == 'S' || input == 'A')
        {
            command = input;
            lastTime = millis();
        }
    }

    // Stop if no new command is received
    if (millis() - lastTime > TIMEOUT)
    {
        command = 'S';
    }

    // Read the sensors
    sensors.read(sensor_values);

    // Check left sensor
    if (sensor_values[0] > QTR_THRESHOLD)
    {
        buzzer.playNote(NOTE_G(3), 100, 15);

        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);

        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(TURN_DURATION);
    }

    // Check right sensor
    else if (sensor_values[5] > QTR_THRESHOLD)
    {
        buzzer.playNote(NOTE_G(3), 100, 15);

        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);

        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        delay(TURN_DURATION);
    }

    // Move forward
    else if (command == 'U')
    {
        motors.setSpeeds(DRIVE_SPEED, DRIVE_SPEED);
    }

    // Move backward
    else if (command == 'D')
    {
        motors.setSpeeds(-DRIVE_SPEED, -DRIVE_SPEED);
    }

    // Turn left
    else if (command == 'L')
    {
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    }

    // Turn right
    else if (command == 'R')
    {
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    }

    // Spin in place
    else if (command == 'A')
    {
        motors.setSpeeds(400, -400);
    }

    // Stop the motors
    else
    {
        motors.setSpeeds(0, 0);
    }
}