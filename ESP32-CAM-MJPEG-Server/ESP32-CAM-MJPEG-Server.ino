#include "esp32-hal-log.h"
#include <Arduino.h>
#include "message.h"

#include "soc/soc.h"             // disable brownout problems
#include "soc/rtc_cntl_reg.h"    // disable brownout problems

#include "ap_control.h"
#include "app_httpd.h"
#include "camera_control.h"

void process(const String &cmd);

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  initCamera();

   initSoftAP();
   startCameraServer();
}

void loop() {
  //
}
