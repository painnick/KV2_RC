set(srcs
        "main.c"
        "PadController.h"
        "PadController.cpp"
        "TrackController.h"
        "TrackController.cpp"
        "TurretController.h"
        "TurretController.cpp"
        )

set(requires "bluepad32")

set(ENV{BLUEPAD32_ARDUINO} TRUE)

# See: https://gitlab.com/ricardoquesada/bluepad32/-/issues/9
if(DEFINED ENV{BLUEPAD32_ARDUINO})
    list(APPEND srcs "arduino_main.cpp")
    list(APPEND requires "bluepad32_arduino" "arduino")
endif()

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}" "ESP32Servo")
