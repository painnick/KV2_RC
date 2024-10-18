//
// Created by painnick on 2022-12-15.
//

#include <Arduino.h>

#include "PadController.h"
#include "common.h"

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

uni_gamepad_t lastPadStates[BP32_MAX_GAMEPADS];

GamepadConnectedEventCallback onGamepadConnected;
GamepadDisconnectedEventCallback onGamepadDisconnected;

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
            Console.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n", gp->getModelName().c_str(),
                           properties.vendor_id, properties.product_id);
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

    onGamepadConnected(gp);
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

    onGamepadDisconnected(gp);
}

PadController::PadController(Bluepad32* bp32) {
    this->bp32_ = bp32;
}

void PadController::setup(const GamepadEventCallback& e,
                          const GamepadConnectedEventCallback& onConnected,
                          const GamepadDisconnectedEventCallback& onDisconnected) {
    this->bp32_->setup(onConnectedGamepad, onDisconnectedGamepad);
    this->onEvent = e;

    onGamepadConnected = onConnected;
    onGamepadDisconnected = onDisconnected;
}

void PadController::loop() {
    bool dataUpdated = false;
    if (this->bp32_ != nullptr) {
        dataUpdated = this->bp32_->update();
    }
    if (!dataUpdated)
        return;

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr myGamepad = myGamepads[i];
        PadEvents events;
        memset(&events, 0, sizeof(PadEvents));

        if (myGamepad && myGamepad->isConnected()) {
            bool modified = false;
            if (lastPadStates[i].dpad != myGamepad->dpad()) {
                events.keyupUp = ((lastPadStates[i].dpad & 0x01) == 0) && ((myGamepad->dpad() & 0x01) > 0);
                events.keyupDown = ((lastPadStates[i].dpad & 0x02) == 0) && ((myGamepad->dpad() & 0x02) > 0);
                events.keyupRight = ((lastPadStates[i].dpad & 0x04) == 0) && ((myGamepad->dpad() & 0x04) > 0);
                events.keyupLeft = ((lastPadStates[i].dpad & 0x08) == 0) && ((myGamepad->dpad() & 0x08) > 0);
                lastPadStates[i].dpad = myGamepad->dpad();
                modified = true;
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
                modified = true;
            }

            int32_t newAxisX = myGamepad->axisX();
            if ((STICK_THRESHOLD_MIN < newAxisX) && (newAxisX < STICK_THRESHOLD_MAX))
                newAxisX = 0;
            if (lastPadStates[i].axis_x != newAxisX) {
                events.changedAxisLx = true;
                lastPadStates[i].axis_x = newAxisX;
                modified = true;
            }

            int32_t newAxisY = myGamepad->axisY();
            if ((STICK_THRESHOLD_MIN < newAxisY) && (newAxisY < STICK_THRESHOLD_MAX))
                newAxisY = 0;
            if (lastPadStates[i].axis_y != newAxisY) {
                events.changedAxisLy = true;
                lastPadStates[i].axis_y = newAxisY;
                modified = true;
            }

            if (lastPadStates[i].axis_rx != myGamepad->axisRX()) {
                events.changedAxisRx = true;
                lastPadStates[i].axis_rx = myGamepad->axisRX();
                modified = true;
            }

            if (lastPadStates[i].axis_ry != myGamepad->axisRY()) {
                events.changedAxisRy = true;
                lastPadStates[i].axis_ry = myGamepad->axisRY();
                modified = true;
            }

            if (lastPadStates[i].misc_buttons != myGamepad->miscButtons()) {
                events.keyupSelect =
                    ((lastPadStates[i].misc_buttons & 0x02) == 0) && ((myGamepad->miscButtons() & 0x02) > 0);
                events.keyupStart =
                    ((lastPadStates[i].misc_buttons & 0x04) == 0) && ((myGamepad->miscButtons() & 0x04) > 0);
                lastPadStates[i].misc_buttons = myGamepad->miscButtons();
                modified = true;
            }

            if (modified) {
                this->onEvent(i, events, myGamepad);
                Console.println("modified");
            }
        }
    }
}