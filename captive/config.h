#ifndef CONFIG_H
#define CONFIG_H

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include "esp_wifi.h"
#include <NimBLEDevice.h>
#include <Preferences.h>
#include <vector>

// LED pins for ESP32 WROOM-32
#define RED_LED 2
#define BLUE_LED 4

const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;

const uint8_t channels[] = {1, 3, 6, 9, 11, 13};
const bool wpa2 = true;

extern String MAIN_SSID;
extern String MAIN_PASS;
extern IPAddress APIP;

extern String customSSIDs;
extern bool beaconTargetMode;

extern uint8_t beaconPacket[109];

#define DEAUTH_FRAME_SIZE 26
#define MAX_STATIONS 20
#define ATTACK_INTERVAL 10

struct Station {
  uint8_t mac[6];
  unsigned long lastSeen;
};

extern bool printerAttackActive;
extern String discoveredPrinters;
extern String printerMessage;

// LED control functions
void initLEDs();
void updateLEDs();

// AP Settings Functions
void loadAPSettings();
void saveAPSettings();

#endif
