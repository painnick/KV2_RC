//
// Created by painnick on 2022-12-13.
//

#ifndef TANK_IDF_TRACKCONTROLLER_H
#define TANK_IDF_TRACKCONTROLLER_H

/**
 * LEDC Chan to Group/Channel/Timer Mapping
 * ledc: 0  => Group: 0, Channel: 0, Timer: 0
 * ledc: 1  => Group: 0, Channel: 1, Timer: 0
 * ledc: 2  => Group: 0, Channel: 2, Timer: 1
 * ledc: 3  => Group: 0, Channel: 3, Timer: 1
 * ledc: 4  => Group: 0, Channel: 4, Timer: 2
 * ledc: 5  => Group: 0, Channel: 5, Timer: 2
 * ledc: 6  => Group: 0, Channel: 6, Timer: 3
 * ledc: 7  => Group: 0, Channel: 7, Timer: 3
 * ledc: 8  => Group: 1, Channel: 0, Timer: 0
 * ledc: 9  => Group: 1, Channel: 1, Timer: 0
 * ledc: 10 => Group: 1, Channel: 2, Timer: 1
 * ledc: 11 => Group: 1, Channel: 3, Timer: 1
 * ledc: 12 => Group: 1, Channel: 4, Timer: 2
 * ledc: 13 => Group: 1, Channel: 5, Timer: 2
 * ledc: 14 => Group: 1, Channel: 6, Timer: 3
 * ledc: 15 => Group: 1, Channel: 7, Timer: 3
 */

class TrackController {
   public:
    TrackController(int pin1, int pin2, int ledc1, int ledc2);
    void forward();
    void backward();
    void stop();
    void setSpeed(int speed);

   protected:
    int ledc1, ledc2;
    int speed;
};

#endif  // TANK_IDF_TRACKCONTROLLER_H
