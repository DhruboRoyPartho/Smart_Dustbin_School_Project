// Code Author: Dhrubo Roy Partho
// Project: Smart Dustbin
// Version: 1.1v
// Date: 09/01/2025
// Components: Arduino, Ultrasonic Sensor, Servo Motors

#include <Servo.h>

Servo lidServo;     // Servo for the dustbin lid
Servo rewardServo;  // Servo for chocolate dispenser

const int trigPin = 10;  // Ultrasonic sensor trigger pin
const int echoPin = 11;  // Ultrasonic sensor echo pin

long duration;
int distance;
const int openAngle = 110;
const int closeAngle = 20;
const int servoSpeed = 5;  // Smaller value = slower movement
const int detectionRange = 25;  // Distance threshold in cm

// Delay in milliseconds
const int lidDelay = 3000;    // ! Change here for lid delay. How much time will stay open the lid?
const int rewardDelay = 300;    // ! Change here for reward delay. How much time will stay open the reward lid?

void measureDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.println(distance);
}

void dispenseReward() {
    rewardServo.write(openAngle);
    delay(rewardDelay);
    rewardServo.write(closeAngle);
}

void smoothServoMove(Servo &servo, int startAngle, int endAngle) {
    if (startAngle < endAngle) {
        for (int pos = startAngle; pos <= endAngle; pos++) {
            servo.write(pos);
            delay(servoSpeed);
        }
    } else {
        for (int pos = startAngle; pos >= endAngle; pos--) {
            servo.write(pos);
            delay(servoSpeed);
        }
    }
}

void setup() {
    Serial.begin(9600);

    lidServo.attach(9);
    lidServo.write(closeAngle);
    rewardServo.attach(6);
    rewardServo.write(closeAngle);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    Serial.println("Smart Dustbin Initialized...");
}

void loop() {
    measureDistance();

    if (distance > 0 && distance <= detectionRange) {
        smoothServoMove(lidServo, closeAngle, openAngle);
        delay(lidDelay);
        smoothServoMove(lidServo, openAngle, closeAngle);
        delay(1000);
        dispenseReward();
        delay(2000);
        
    } else {
        lidServo.write(closeAngle);
    }

    delay(500);
}
