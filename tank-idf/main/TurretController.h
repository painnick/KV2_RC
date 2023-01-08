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
    void turnLeft(int angle = 10);
    void turnRight(int angle = 10);
    void loop();

   protected:
    Servo* servo = NULL;
    int currentAngle = 0;
    int targetAngle = 0;
    int maxLeft = -45, maxRight = 45;
    unsigned long lastMoved = 0;

    void turn(int angle);
};

#endif  // TANK_IDF_TURRETCONTROLLER_H
