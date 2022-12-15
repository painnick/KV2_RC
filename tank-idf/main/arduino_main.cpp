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

#include "PadController.h"
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

PadController pad32(&BP32);

Servo servoTurret;
TurretController turretController(&servoTurret, PIN_TURRET_SERVO);

TrackController leftTrack(PIN_TRACK_L1_MOTOR, PIN_TRACK_L2_MOTOR, CHANNEL_L1, CHANNEL_L2);
TrackController rightTrack(PIN_TRACK_R1_MOTOR, PIN_TRACK_R2_MOTOR, CHANNEL_R1, CHANNEL_R2);

#ifdef USE_SOUND
HardwareSerial mySerial(2);  // 16, 17
DfMp3 dfmp3(mySerial);
int volume = MAX_VOLUME;  // 0~30
#endif

void onReset() {
    ESP_LOGI(MAIN_TAG, "Reset");

    turretController.init();

    leftTrack.stop();
    rightTrack.stop();

#ifdef USE_SOUND
    dfmp3.playMp3FolderTrack(3);
#endif
}

// Arduino setup function. Runs in CPU 1
void setup() {
    pad32.setup();

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But might also fix some connection / re-connection issues.
    // BP32.forgetBluetoothKeys();

    dfmp3.begin();
}

// Arduino loop function. Runs in CPU 1
void loop() {
    dfmp3.loop();

    // This call fetches all the gamepad info from the NINA (ESP32) module.
    // Just call this function in your main loop.
    // The gamepads pointer (the ones received in the callbacks) gets updated
    // automatically.
    pad32.loop();

    delay(10);
}
