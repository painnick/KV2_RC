//
// Created by painnick on 22. 12. 24.
//

#include "CannonController.h"
#include <Arduino.h>

#define DEFAULT_DEGREE (-10)
#define MAX_DEGREE (5)
#define MIN_DEGREE (-30)

CannonController::CannonController(Servo* servo1, int pinNo1, int minPulseWidth, int maxPulseWidth)
    : servo(servo1),
      currentAngle(DEFAULT_DEGREE),
      maxUp(MAX_DEGREE),
      maxDown(MIN_DEGREE) {
    this->servo->attach(pinNo1, minPulseWidth, maxPulseWidth);
}

void CannonController::init() {
    this->currentAngle = DEFAULT_DEGREE;
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
