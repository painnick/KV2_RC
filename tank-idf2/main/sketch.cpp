//
// README FIRST, README FIRST, README FIRST
//
// Bluepad32 has a built-in interactive console.
// By default, it is enabled (hey, this is a great feature!).
// But it is incompatible with Arduino "Serial" class.
//
// Instead of using, "Serial" you can use Bluepad32 "Console" class instead.
// It is somewhat similar to Serial but not exactly the same.
//
// Should you want to still use "Serial", you have to disable the Bluepad32's console
// from "sdkconfig.defaults" with:
//    CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE=n

#include "sdkconfig.h"

#include <Arduino.h>
#include <Bluepad32.h>
#include <ESP32Servo.h>

#include "./common.h"

#include "./controllers/Mp3Notify.h"
#include "./libs/DFMiniMp3.h"

#include "./controllers/CannonController.h"
#include "./controllers/PadController.h"
#include "./controllers/TrackController.h"
#include "./controllers/TurretController.h"

#define MAIN_TAG "Main"

PadController pad32(&BP32);

Servo servoTurret;
TurretController turretController(&servoTurret, PIN_TURRET_SERVO);

Servo servoCannon;
CannonController cannonController(&servoCannon, PIN_CANNON_SERVO, 900, 2100);

TrackController leftTrack(PIN_TRACK_L1_MOTOR, PIN_TRACK_L2_MOTOR, CHANNEL_L1, CHANNEL_L2);
TrackController rightTrack(PIN_TRACK_R1_MOTOR, PIN_TRACK_R2_MOTOR, CHANNEL_R1, CHANNEL_R2);

DfMp3 dfmp3(Serial1, PIN_MP3_RX, PIN_MP3_TX);
bool headLightOn = false;

void onReset() {
    ESP_LOGI(MAIN_TAG, "Reset");

    turretController.init();
    cannonController.init();

    leftTrack.stop();
    rightTrack.stop();

    dfmp3.playMp3FolderTrack(TRACK_RESET);
}

void onPadEvent(int index, PadEvents events, GamepadPtr gamepad) {
    // Cannon Fire
    if (events.keyupA) {
        digitalWrite(PIN_CANNON_LIGHT, HIGH);
        delay(100);
        digitalWrite(PIN_CANNON_LIGHT, LOW);

        leftTrack.backward();
        rightTrack.backward();
        delay(20);

        dfmp3.playMp3FolderTrack(TRACK_CANNON);

        leftTrack.stop();
        rightTrack.stop();
    }
    if (events.keyupB) {
        dfmp3.playMp3FolderTrack(TRACK_GATLING);
    }

    // Cannon Up/Down
    if (events.keyupUp) {
        cannonController.turnUp();
    }
    if (events.keyupDown) {
        cannonController.turnDown();
    }

    // Turret Left/Right
    if (events.keyupLeft) {
        turretController.turnLeft();
    }
    if (events.keyupRight) {
        turretController.turnRight();
    }

    // HeadLight
    if (events.keyupSelect) {
        headLightOn = !headLightOn;
        digitalWrite(PIN_HEAD_LIGHT, headLightOn ? HIGH : LOW);
    }

    // Reset
    if (events.keyupStart) {
        onReset();
    }

    // Volume
    if (events.keyupL1) {
        dfmp3.increaseVolume();
    }
    if (events.keyupL2) {
        dfmp3.decreaseVolume();
    }

    // Speed
    if (events.keyupR1) {
        leftTrack.speedUp();
        rightTrack.speedUp();
    }
    if (events.keyupR2) {
        leftTrack.speedDown();
        rightTrack.speedDown();
    }

    int32_t Ly = gamepad->axisY();
    if (Ly > 50) {
        leftTrack.backward();
    } else if (Ly < -50) {
        leftTrack.forward();
    } else {
        leftTrack.stop();
    }

    int32_t Ry = gamepad->axisRY();
    if (Ry > 50) {
        rightTrack.backward();
    } else if (Ry < -50) {
        rightTrack.forward();
    } else {
        rightTrack.stop();
    }
}

void onPadConnected(GamepadPtr gp) {
    dfmp3.playMp3FolderTrack(TRACK_RESET);
}

void onPadDisonnected(GamepadPtr gp) {
    dfmp3.setVolume(DEFAULT_VOLUME);

    dfmp3.loopGlobalTrack(4);
}

void dumpGamepad(ControllerPtr ctl) {
    Console.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ()        // Accelerometer Z
    );
}

void processGamepad(ControllerPtr ctl) {
    // There are different ways to query whether a button is pressed.
    // By query each button individually:
    //  a(), b(), x(), y(), l1(), etc...
    if (ctl->a()) {
        static int colorIdx = 0;
        // Some gamepads like DS4 and DualSense support changing the color LED.
        // It is possible to change it by calling:
        switch (colorIdx % 3) {
            case 0:
                // Red
                ctl->setColorLED(255, 0, 0);
                break;
            case 1:
                // Green
                ctl->setColorLED(0, 255, 0);
                break;
            case 2:
                // Blue
                ctl->setColorLED(0, 0, 255);
                break;
        }
        colorIdx++;
    }

    if (ctl->b()) {
        // Turn on the 4 LED. Each bit represents one LED.
        static int led = 0;
        led++;
        // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
        // support changing the "Player LEDs": those 4 LEDs that usually indicate
        // the "gamepad seat".
        // It is possible to change them by calling:
        ctl->setPlayerLEDs(led & 0x0f);
    }

    if (ctl->x()) {
        // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S, Stadia support rumble.
        // It is possible to set it by calling:
        // Some controllers have two motors: "strong motor", "weak motor".
        // It is possible to control them independently.
        ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
                            0x40 /* strongMagnitude */);
    }

    // Another way to query controller data is by getting the buttons() function.
    // See how the different "dump*" functions dump the Controller info.
    dumpGamepad(ctl);

    // See ArduinoController.h for all the available functions.
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Console.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Console.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    // Setup the Bluepad32 callbacks
    pad32.setup(onPadEvent, onPadConnected, onPadDisonnected);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But it might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();

    // Enables mouse / touchpad support for gamepads that support them.
    // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
    // - First one: the gamepad
    // - Second one, which is a "virtual device", is a mouse.
    // By default, it is disabled.
    BP32.enableVirtualDevice(false);

    // Enables the BLE Service in Bluepad32.
    // This service allows clients, like a mobile app, to setup and see the state of Bluepad32.
    // By default, it is disabled.
    BP32.enableBLEService(false);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
    // This call fetches all the controllers' data.
    // Call this function in your main loop.
    pad32.loop();

    // The main loop must have some kind of "yield to lower priority task" event.
    // Otherwise, the watchdog will get triggered.
    // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
    // Detailed info here:
    // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

    //     vTaskDelay(1);
    delay(150);
    dfmp3.loop();
}
