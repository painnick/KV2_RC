//
// Created by painnick on 22. 12. 24.
//

#ifndef TANK_IDF_CANNONCONTROLLER_H
#define TANK_IDF_CANNONCONTROLLER_H

#include <ESP32Servo.h>

class CannonController {
   public:
    CannonController(Servo* servo, int pinNo, int minPulseWidth = 500, int maxPulseWidth = 2500);
    void init();
    int turnUp(int angle = 2);
    int turnDown(int angle = 2);

   protected:
    Servo* servo = NULL;
    int currentAngle;
    int maxUp, maxDown;

    void turn(int angle);
};

#endif  // TANK_IDF_CANNONCONTROLLER_H
