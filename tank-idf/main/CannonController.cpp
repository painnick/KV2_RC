//
// Created by painnick on 22. 12. 24.
//

#include "CannonController.h"
#include <Arduino.h>

CannonController::CannonController(Servo* servo1, int pinNo1, int minPulseWidth, int maxPulseWidth)
    : servo(servo1),
      currentAngle(5),
      maxUp(20),
      maxDown(0) {
    this->servo->attach(pinNo1, minPulseWidth, maxPulseWidth);
}

void CannonController::init() {
    this->currentAngle = 0;
    this->turn(this->currentAngle);
}

int CannonController::turnUp(int angle) {
    this->currentAngle = min(this->currentAngle + angle, this->maxUp);
    this->turn(this->currentAngle);
    return this->currentAngle;
}

int CannonController::turnDown(int angle) {
    this->currentAngle = max(this->currentAngle - angle, this->maxDown);
    this->turn(this->currentAngle);
    return this->currentAngle;
}

void CannonController::turn(int angle) {
    this->servo->write(angle + 90);
}
