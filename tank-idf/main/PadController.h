//
// Created by painnick on 2022-12-15.
//

#ifndef TANK_IDF_PADCONTROLLER_H
#define TANK_IDF_PADCONTROLLER_H

#include <Bluepad32.h>

typedef struct {
    // buttons
    bool keyupA;   // 0x01
    bool keyupB;   // 0x02
    bool keyupX;   // 0x04
    bool keyupY;   // 0x08
    bool keyupL1;  // 0x10
    bool keyupR1;  // 0x20
    bool keyupL2;  // 0x40
    bool keyupR2;  // 0x80
    // dpad
    bool keyupUp;     // 0x01
    bool keyupDown;   // 0x02
    bool keyupRight;  // 0x04
    bool keyupLeft;   // 0x08
    // misc
    bool keyupSelect;  // 0x02
    bool keyupStart;   // 0x04
    //
    bool changedAxisLx;
    bool changedAxisLy;
    bool changedAxisRx;
    bool changedAxisRy;
} PadEvents;

typedef std::function<void(int index, PadEvents events, GamepadPtr gamepad)> GamepadEventCallback;

class PadController : Bluepad32 {
   public:
    PadController(Bluepad32* bp32);
    void setup(const GamepadEventCallback onEvent);
    void loop();

   protected:
    Bluepad32* bp32_ = nullptr;
    GamepadEventCallback onEvent;
};

#endif  // TANK_IDF_PADCONTROLLER_H
