//
// Created by painnick on 2022-12-13.
//

#include <Arduino.h>
#include "TurretController.h"

TurretController::TurretController(Servo* servo1, int pinNo1, int minPulseWidth, int maxPulseWidth)
    : servo(servo1),
      currentAngle(0),
      targetAngle(0),
      maxLeft(-45),
      maxRight(45),
      lastMoved(0) {
    this->servo->attach(pinNo1, minPulseWidth, maxPulseWidth);
}

void TurretController::init() {
    this->targetAngle = 0;
}

void TurretController::turnLeft(int angle) {
    this->targetAngle = min(this->currentAngle + angle, this->maxRight);
}

void TurretController::turnRight(int angle) {
    this->targetAngle = max(this->currentAngle - angle, this->maxLeft);
}

void TurretController::turn(int angle) {
    this->servo->write(angle + 90);
}

void TurretController::loop() {
    unsigned long now = millis();

    if (now - this->lastMoved > 30) {
        if (this->targetAngle < this->currentAngle) {
            this->currentAngle = this->currentAngle - 1;
            this->turn(this->currentAngle);
        }
        else if (this->targetAngle > this->currentAngle) {
            this->currentAngle = this->currentAngle + 1;
            this->turn(this->currentAngle);
        }
       this->lastMoved = now;
    }
}
