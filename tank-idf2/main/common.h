//
// Created by painnick on 2024-10-16.
//
#ifndef TANK_IDF2_MAIN_COMMON_H_
#define TANK_IDF2_MAIN_COMMON_H_

// These are all GPIO pins on the ESP32
// Recommended pins include 2,4,12-19,21-23,25-27,32-33
// for the ESP32-S2 the GPIO pins are 1-21,26,33-42

#define PIN_MP3_RX 32
#define PIN_MP3_TX 33

#define PIN_CANNON_SERVO 21  // PWM(Servo)
#define PIN_TURRET_SERVO 22  // PWM(Servo)

#define PIN_TRACK_L1_MOTOR 18  // PWM(Analog)
#define PIN_TRACK_L2_MOTOR 26  // PWM(Analog)
#define PIN_TRACK_R1_MOTOR 19  // PWM(Analog)
#define PIN_TRACK_R2_MOTOR 23  // PWM(Analog)

#define PIN_CANNON_LIGHT 27
#define PIN_HEAD_LIGHT 25

#define CHANNEL_L1 14
#define CHANNEL_L2 15
#define CHANNEL_R1 12
#define CHANNEL_R2 13

#define DEFAULT_VOLUME 18

#define STICK_THRESHOLD_MIN -20
#define STICK_THRESHOLD_MAX 20

#define TRACK_MOTOR_RESOLUTION 8

#define TRACK_WAIT 1
#define TRACK_CANNON 2
#define TRACK_GATLING 3
#define TRACK_RESET 4

#endif  // TANK_IDF2_MAIN_COMMON_H_
