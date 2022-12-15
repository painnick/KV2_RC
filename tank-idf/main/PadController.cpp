//
// Created by painnick on 2022-12-15.
//

#include <Arduino.h>

#include "PadController.h"

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

uni_gamepad_t lastPadStates[BP32_MAX_GAMEPADS];

typedef struct {
    // buttons
    bool keyupA; // 0x01
    bool keyupB; // 0x02
    bool keyupX; // 0x04
    bool keyupY; // 0x08
    bool keyupL1; // 0x10
    bool keyupR1; // 0x20
    bool keyupL2; // 0x40
    bool keyupR2; // 0x80
    // dpad
    bool keyupUp; // 0x01
    bool keyupDown; // 0x02
    bool keyupRight; // 0x04
    bool keyupLeft; // 0x08
    // misc
    bool keyupSelect; // 0x02
    bool keyupStart; // 0x04
    //
    bool changedAxisLx;
    bool changedAxisLy;
    bool changedAxisRx;
    bool changedAxisRy;
} PadEvents;

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == nullptr) {
            Console.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            GamepadProperties properties = gp->getProperties();
            Console.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n", gp->getModelName(), properties.vendor_id,
                           properties.product_id);
            myGamepads[i] = gp;
            foundEmptySlot = true;

            // TODO 연결 성공음 재생
            break;
        }
    }
    if (!foundEmptySlot) {
        Console.println("CALLBACK: Gamepad connected, but could not found empty slot");
        // TODO 연결 실패음 재생
    }
}

void onDisconnectedGamepad(GamepadPtr gp) {
    bool foundGamepad = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == gp) {
            Console.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
            myGamepads[i] = nullptr;
            lastPadStates[i].buttons = 0;
            lastPadStates[i].dpad = 0;
            lastPadStates[i].buttons = 0;
            lastPadStates[i].axis_x = 0;
            lastPadStates[i].axis_y = 0;
            lastPadStates[i].axis_rx = 0;
            lastPadStates[i].axis_ry = 0;
            lastPadStates[i].buttons = 0;
            lastPadStates[i].misc_buttons = 0;
            // TODO 연결 해지음 재생
            foundGamepad = true;
            break;
        }
    }

    if (!foundGamepad) {
        Console.println("CALLBACK: Gamepad disconnected, but not found in myGamepads");
    }
}

PadController::PadController(Bluepad32* bp32) {
    this->bp32_ = bp32;
}

void PadController::setup() {
    this->bp32_->setup(onConnectedGamepad, onDisconnectedGamepad);
}

void PadController::loop() {
    if (this->bp32_ != nullptr) {
        this->bp32_->update();
    }

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr myGamepad = myGamepads[i];
        PadEvents events;
        memset(&events, 0, sizeof(PadEvents));

        bool changed = false;
        if (myGamepad && myGamepad->isConnected()) {
            if (lastPadStates[i].dpad != myGamepad->dpad()) {
                events.keyupUp = ((lastPadStates[i].dpad & 0x01) == 0) && ((myGamepad->dpad() & 0x01) > 0);
                events.keyupDown = ((lastPadStates[i].dpad & 0x02) == 0) && ((myGamepad->dpad() & 0x02) > 0);
                events.keyupRight = ((lastPadStates[i].dpad & 0x04) == 0) && ((myGamepad->dpad() & 0x04) > 0);
                events.keyupLeft = ((lastPadStates[i].dpad & 0x08) == 0) && ((myGamepad->dpad() & 0x08) > 0);
                lastPadStates[i].dpad = myGamepad->dpad();
                changed = true;
            }

            if (lastPadStates[i].buttons != myGamepad->buttons()) {
                events.keyupA = ((lastPadStates[i].buttons & 0x01) == 0) && ((myGamepad->buttons() & 0x01) > 0);
                events.keyupB = ((lastPadStates[i].buttons & 0x02) == 0) && ((myGamepad->buttons() & 0x02) > 0);
                events.keyupX = ((lastPadStates[i].buttons & 0x04) == 0) && ((myGamepad->buttons() & 0x04) > 0);
                events.keyupY = ((lastPadStates[i].buttons & 0x08) == 0) && ((myGamepad->buttons() & 0x08) > 0);
                events.keyupL1 = ((lastPadStates[i].buttons & 0x10) == 0) && ((myGamepad->buttons() & 0x10) > 0);
                events.keyupR1 = ((lastPadStates[i].buttons & 0x20) == 0) && ((myGamepad->buttons() & 0x20) > 0);
                events.keyupL2 = ((lastPadStates[i].buttons & 0x40) == 0) && ((myGamepad->buttons() & 0x40) > 0);
                events.keyupR2 = ((lastPadStates[i].buttons & 0x80) == 0) && ((myGamepad->buttons() & 0x80) > 0);
                lastPadStates[i].buttons = myGamepad->buttons();
                changed = true;
            }

            if (lastPadStates[i].axis_x != myGamepad->axisX()) {
                events.changedAxisLx = true;
                lastPadStates[i].axis_x = myGamepad->axisX();
                changed = true;
            }

            if (lastPadStates[i].axis_y != myGamepad->axisY()) {
                events.changedAxisLy = true;
                lastPadStates[i].axis_y = myGamepad->axisY();
                changed = true;
            }

            if (lastPadStates[i].axis_rx != myGamepad->axisRX()) {
                events.changedAxisRx = true;
                lastPadStates[i].axis_rx = myGamepad->axisRX();
                changed = true;
            }

            if (lastPadStates[i].axis_ry != myGamepad->axisRY()) {
                events.changedAxisRy = true;
                lastPadStates[i].axis_ry = myGamepad->axisRY();
                changed = true;
            }

            if (lastPadStates[i].misc_buttons != myGamepad->miscButtons()) {
                events.keyupSelect = ((lastPadStates[i].misc_buttons & 0x02) == 0) && ((myGamepad->miscButtons() & 0x02) > 0);
                events.keyupStart = ((lastPadStates[i].misc_buttons & 0x04) == 0) && ((myGamepad->miscButtons() & 0x04) > 0);
                lastPadStates[i].misc_buttons = myGamepad->miscButtons();
                changed = true;
            }
        }

        if (changed) {
            if (events.keyupUp)
                Console.println("Up");
            if (events.keyupDown)
                Console.println("Down");
            if (events.keyupLeft)
                Console.println("Left");
            if (events.keyupRight)
                Console.println("Right");

            if (events.keyupA)
                Console.println("A");
            if (events.keyupB)
                Console.println("B");
            if (events.keyupX)
                Console.println("X");
            if (events.keyupY)
                Console.println("Y");

            if (events.keyupL1)
                Console.println("L1");
            if (events.keyupR1)
                Console.println("R1");
            if (events.keyupL2)
                Console.println("L2");
            if (events.keyupR2)
                Console.println("R2");

            if (events.keyupSelect)
                Console.println("Select");
            if (events.keyupStart)
                Console.println("Start");

            if (events.changedAxisLx)
                Console.printf("Lx : %4d\n", myGamepad->axisX());
            if (events.changedAxisLy)
                Console.printf("Ly : %4d\n", myGamepad->axisY());
            if (events.changedAxisRx)
                Console.printf("Rx : %4d\n", myGamepad->axisRX());
            if (events.changedAxisRy)
                Console.printf("Ry : %4d\n", myGamepad->axisRY());
        }
    }

}