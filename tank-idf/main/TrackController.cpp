//
// Created by painnick on 2022-12-13.
//

#include <Arduino.h>
#include "TrackController.h"

// 0~255
#define TRACK_MOTOR_RESOLUTION 8

TrackController::TrackController(int pin1, int pin2, int ledc1, int ledc2) : ledc1(ledc1), ledc2(ledc2), speed(255) {
    ledcSetup(pin1, 1000, TRACK_MOTOR_RESOLUTION);
    ledcSetup(pin2, 1000, TRACK_MOTOR_RESOLUTION);

    ledcAttachPin(pin1, ledc1);
    ledcAttachPin(pin2, ledc2);
}

void TrackController::forward() {
    ledcWrite(ledc1, speed);
    ledcWrite(ledc2, 0);
}

void TrackController::backward() {
    ledcWrite(ledc1, 0);
    ledcWrite(ledc2, speed);
}

void TrackController::stop() {
    ledcWrite(ledc1, 0);
    ledcWrite(ledc2, 0);
}

void TrackController::setSpeed(int speed1) {
    speed = speed1;
}
