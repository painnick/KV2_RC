/****************************************************************************
README FIRST, README FIRST, README FIRST

Bluepad32 has a built-in interactive console.
By default it is enabled (hey, this is a great feature!).
But it is incompatible with Arduino "Serial" class.

Instead of using "Serial" you can use Bluepad32 "Console" class instead.
It is somewhat similar to Serial but not exactly the same.

Should you want to still use "Serial", you have to disable the Bluepad32's console
from "sdkconfig.defaults" with:
CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE=n

https://gitlab.com/ricardoquesada/bluepad32/-/blob/main/docs/plat_arduino.md
https://gitlab.com/ricardoquesada/bluepad32/-/blob/main/docs/supported_gamepads.md
****************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#define MAIN_TAG "Main"

#define USE_SOUND

#include <Arduino.h>
#include <Bluepad32.h>
#include <ESP32Servo.h>

#ifdef USE_SOUND
#include "DFMiniMp3.h"
#include "Mp3Notify.h"
#endif

#include "TurretController.h"
#include "TrackController.h"

// These are all GPIO pins on the ESP32
// Recommended pins include 2,4,12-19,21-23,25-27,32-33
// for the ESP32-S2 the GPIO pins are 1-21,26,33-42

// 13 outputs PWM signal at boot
// 14 outputs PWM signal at boot

#ifdef USE_SOUND
#define PIN_RX 16  // RX2
#define PIN_TX 17  // TX2
#endif

#define PIN_TURRET_SERVO 22  // PWM(Servo)

#define PIN_MISSILE_LED 23  // Digital

#define PIN_TRACK_L1_MOTOR 33  // PWM(Analog)
#define PIN_TRACK_L2_MOTOR 32  // PWM(Analog)
#define PIN_TRACK_R1_MOTOR 25  // PWM(Analog)
#define PIN_TRACK_R2_MOTOR 26  // PWM(Analog)

#define CHANNEL_L1 14
#define CHANNEL_L2 15
#define CHANNEL_R1 12
#define CHANNEL_R2 13

#ifdef USE_SOUND
#define MAX_VOLUME 18
#endif

#define STICK_THRESHOLD 20

#define TRACK_MOTOR_RESOLUTION 7

Servo servoTurret;
TurretController turretController(&servoTurret, PIN_TURRET_SERVO);

TrackController leftTrack(PIN_TRACK_L1_MOTOR, PIN_TRACK_L2_MOTOR, CHANNEL_L1, CHANNEL_L2);
TrackController rightTrack(PIN_TRACK_R1_MOTOR, PIN_TRACK_R2_MOTOR, CHANNEL_R1, CHANNEL_R2);

#ifdef USE_SOUND
HardwareSerial mySerial(2);  // 16, 17
DfMp3 dfmp3(mySerial);
int volume = MAX_VOLUME;  // 0~30
#endif

bool isTurbo = false;

bool circlePress = false;
bool trianglePress = false;
bool squarePress = false;
bool crossPress = false;

void reset() {
    ESP_LOGI(MAIN_TAG, "Reset");

    turretController.init();

    leftTrack.stop();
    rightTrack.stop();

#ifdef USE_SOUND
    dfmp3.playMp3FolderTrack(3);
#endif
}

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

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
            break;
        }
    }
    if (!foundEmptySlot) {
        Console.println("CALLBACK: Gamepad connected, but could not found empty slot");
    }
}

void onDisconnectedGamepad(GamepadPtr gp) {
    bool foundGamepad = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == gp) {
            Console.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
            myGamepads[i] = nullptr;
            foundGamepad = true;
            break;
        }
    }

    if (!foundGamepad) {
        Console.println("CALLBACK: Gamepad disconnected, but not found in myGamepads");
    }
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Console.printf("Firmware: %s\n", BP32.firmwareVersion());

    // Set up the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();
}

// Arduino loop function. Runs in CPU 1
void loop() {
    // This call fetches all the gamepad info from the NINA (ESP32) module.
    // Just call this function in your main loop.
    // The gamepads pointer (the ones received in the callbacks) gets updated
    // automatically.
    BP32.update();

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr myGamepad = myGamepads[i];

        if (myGamepad && myGamepad->isConnected()) {
            // There are different ways to query whether a button is pressed.
            // By query each button individually:
            //  a(), b(), x(), y(), l1(), etc...
            if (myGamepad->a()) {
                static int colorIdx = 0;
                // Some gamepads like DS4 and DualSense support changing the color LED.
                // It is possible to change it by calling:
                switch (colorIdx % 3) {
                    case 0:
                        // Red
                        myGamepad->setColorLED(255, 0, 0);
                        break;
                    case 1:
                        // Green
                        myGamepad->setColorLED(0, 255, 0);
                        break;
                    case 2:
                        // Blue
                        myGamepad->setColorLED(0, 0, 255);
                        break;
                }
                colorIdx++;
            }

            if (myGamepad->b()) {
                // Turn on the 4 LED. Each bit represents one LED.
                static int led = 0;
                led++;
                // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
                // support changing the "Player LEDs": those 4 LEDs that usually indicate
                // the "gamepad seat".
                // It is possible to change them by calling:
                myGamepad->setPlayerLEDs(led & 0x0f);
            }

            if (myGamepad->x()) {
                // Duration: 255 is ~2 seconds
                // force: intensity
                // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S support
                // rumble.
                // It is possible to set it by calling:
                myGamepad->setRumble(0xc0 /* force */, 0xc0 /* duration */);
            }

            // Another way to query the buttons, is by calling buttons(), or
            // miscButtons() which return a bitmask.
            // Some gamepads also have DPAD, axis and more.
            Console.printf(
                "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, "
                "%4d, brake: %4d, throttle: %4d, misc: 0x%02x\n",
                i,                        // Gamepad Index
                myGamepad->dpad(),        // DPAD
                myGamepad->buttons(),     // bitmask of pressed buttons
                myGamepad->axisX(),       // (-511 - 512) left X Axis
                myGamepad->axisY(),       // (-511 - 512) left Y axis
                myGamepad->axisRX(),      // (-511 - 512) right X axis
                myGamepad->axisRY(),      // (-511 - 512) right Y axis
                myGamepad->brake(),       // (0 - 1023): brake button
                myGamepad->throttle(),    // (0 - 1023): throttle (AKA gas) button
                myGamepad->miscButtons()  // bitmask of pressed "misc" buttons
            );

            // You can query the axis and other properties as well. See Gamepad.h
            // For all the available functions.
        }
    }

    delay(150);
}
