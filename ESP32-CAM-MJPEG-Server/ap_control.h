#pragma once

#include <Arduino.h>
#include <WiFi.h>

void initSoftAP();
void closeSoftAP();
bool isAPOn();