#include "ap_control.h"

// ===========================
// Enter your WiFi credentials
// ===========================
const char *ssid = "esp32cam_kv2";
const char *password = "aaaa1111";

#define SOFT_AP_SSID "esp32cam_kv2"
#define SOFT_AP_PASSWORD "aaaa1111"

#define SOFT_AP_IP_ADDRESS_1 192
#define SOFT_AP_IP_ADDRESS_2 168
#define SOFT_AP_IP_ADDRESS_3 5
#define SOFT_AP_IP_ADDRESS_4 18

#define SOFT_AP_GW_ADDRESS_1 192
#define SOFT_AP_GW_ADDRESS_2 168
#define SOFT_AP_GW_ADDRESS_3 1
#define SOFT_AP_GW_ADDRESS_4 10

#define SOFT_AP_NM_ADDRESS_1 255
#define SOFT_AP_NM_ADDRESS_2 255
#define SOFT_AP_NM_ADDRESS_3 255
#define SOFT_AP_NM_ADDRESS_4 0

bool _isAPOn = false;

void initSoftAP() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  IPAddress local_ip(SOFT_AP_IP_ADDRESS_1, SOFT_AP_IP_ADDRESS_2, SOFT_AP_IP_ADDRESS_3, SOFT_AP_IP_ADDRESS_4);
  IPAddress gateway(SOFT_AP_GW_ADDRESS_1, SOFT_AP_GW_ADDRESS_2, SOFT_AP_GW_ADDRESS_3, SOFT_AP_GW_ADDRESS_4);
  IPAddress subnet(SOFT_AP_NM_ADDRESS_1, SOFT_AP_NM_ADDRESS_2, SOFT_AP_NM_ADDRESS_3, SOFT_AP_NM_ADDRESS_4);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(SOFT_AP_SSID /*, SOFT_AP_PASSWORD */);
  WiFi.setSleep(false);

  _isAPOn = true;

  log_i("Turn on SoftAP");
}

void closeSoftAP() {
  WiFi.softAPdisconnect(true);
  _isAPOn = false;
  log_i("Turn off SoftAP");
}

bool isAPOn() {
  return _isAPOn;
}