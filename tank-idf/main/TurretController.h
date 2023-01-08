//
// Created by painnick on 2022-12-13.
//

#ifndef TANK_IDF_TURRETCONTROLLER_H
#define TANK_IDF_TURRETCONTROLLER_H

#include <ESP32Servo.h>

class TurretController {
   public:
    TurretController(Servo* servo, int pinNo, int minPulseWidth = 500, int maxPulseWidth = 2500);
    void init();
    int turnLeft(int angle = 5);
    int turnRight(int angle = 5);

   protected:
    Servo* servo = NULL;
    int currentAngle;
    int maxLeft, maxRight;

    void turn(int angle);
};

#endif  // TANK_IDF_TURRETCONTROLLER_H
