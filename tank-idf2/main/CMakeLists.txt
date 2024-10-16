set(srcs
        "main.c"
        "sketch.cpp"
        "libs/DFMiniMp3.h"
        "controllers/CannonController.h"
        "controllers/CannonController.cpp"
        "controllers/Mp3Controller.h"
        "controllers/PadController.h"
        "controllers/PadController.cpp"
        "controllers/TrackController.h"
        "controllers/TrackController.cpp"
        "controllers/TurretController.h"
        "controllers/TurretController.cpp"
)

set(requires "bluepad32" "bluepad32_arduino" "arduino" "btstack" "ESP32Servo")

idf_component_register(SRCS "${srcs}"
        INCLUDE_DIRS "."
        REQUIRES "${requires}")
