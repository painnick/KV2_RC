//
// Created by painnick on 2022-12-13.
//

#include <Arduino.h>
#include "TrackController.h"

TrackController::TrackController(int pin1, int pin2, int ledc1, int ledc2) : ledc1(ledc1), ledc2(ledc2), speed(256 - 1) {
    ledcSetup(ledc1, 1000, TRACK_MOTOR_RESOLUTION);
    ledcSetup(ledc2, 1000, TRACK_MOTOR_RESOLUTION);

    ledcAttachPin(pin1, ledc1);
    ledcAttachPin(pin2, ledc2);
}

void TrackController::forward() const {
    ledcWrite(ledc1, speed);
    ledcWrite(ledc2, 0);
}

void TrackController::backward() const {
    ledcWrite(ledc1, 0);
    ledcWrite(ledc2, speed);
}

void TrackController::stop() const {
    ledcWrite(ledc1, 0);
    ledcWrite(ledc2, 0);
}

void TrackController::setSpeed(int speed1) {
    speed = speed1;
}

int TrackController::speedUp(int step) {
    speed = min(speed + step, 256 - 1);
    return speed;
}

int TrackController::speedDown(int step) {
    speed = max(speed - step, 196 - 1);
    return speed;
}