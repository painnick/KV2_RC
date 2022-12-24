//
// Created by painnick on 2022-12-13.
//

#include <Arduino.h>
#include "TurretController.h"

TurretController::TurretController(Servo* servo1, int pinNo1, int minPulseWidth, int maxPulseWidth)
    : servo(servo1),
      currentAngle(0),
      maxLeft(-30),
      maxRight(30) {
    this->servo->attach(pinNo1, minPulseWidth, maxPulseWidth);
}

void TurretController::init() {
    this->currentAngle = 0;
    this->turn(this->currentAngle);
}

int TurretController::turnLeft(int angle) {
    this->currentAngle = max(this->currentAngle - angle, this->maxLeft);
    this->turn(this->currentAngle);
    return this->currentAngle;
}

int TurretController::turnRight(int angle) {
    this->currentAngle = min(this->currentAngle + angle, this->maxRight);
    this->turn(this->currentAngle);
    return this->currentAngle;
}

void TurretController::turn(int angle) {
    this->servo->write(angle + 90);
}
