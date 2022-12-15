//
// Created by painnick on 2022-12-15.
//

#ifndef TANK_IDF_PADCONTROLLER_H
#define TANK_IDF_PADCONTROLLER_H

#include <Bluepad32.h>

class PadController : Bluepad32 {
   public:
    PadController(Bluepad32* bp32);
    void setup();
    void loop();
   protected:
    Bluepad32* bp32_ = nullptr;
};

#endif  // TANK_IDF_PADCONTROLLER_H
