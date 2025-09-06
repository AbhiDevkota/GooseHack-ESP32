#ifndef CONFIG_H
#define CONFIG_H

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include "esp_wifi.h"
#include <NimBLEDevice.h>
#include <Preferences.h>

String MAIN_SSID = "Durgesh ko bhai Hagess";
String MAIN_PASS = "Bruh1234@#";
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
IPAddress APIP(192, 168, 4, 1);

const uint8_t channels[] = {1, 3, 6, 9, 11, 13};
const bool wpa2 = true;
String customSSIDs = "";
bool beaconTargetMode = false;

uint8_t beaconPacket[109] = {
  0x80, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
  0x00, 0x00,
  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00,
  0xe8, 0x03,
  0x31, 0x00,
  0x00, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x01, 0x08,
  0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c,
  0x03, 0x01,
  0x01,
  0x30, 0x18,
  0x01, 0x00,
  0x00, 0x0f, 0xac, 0x02,
  0x02, 0x00,
  0x00, 0x0f, 0xac, 0x04, 0x00, 0x0f, 0xac, 0x04,
  0x01, 0x00,
  0x00, 0x0f, 0xac, 0x02,
  0x00, 0x00
};

#define DEAUTH_FRAME_SIZE 26
#define MAX_STATIONS 20
#define ATTACK_INTERVAL 10

struct Station {
  uint8_t mac[6];
  unsigned long lastSeen;
};

extern String targetSSID;
extern int targetChannel;
extern uint8_t targetBSSID[6];
extern bool attackActive;
extern bool evilActive;
extern bool sourAppleActive;
extern bool combinedActive;
extern bool windowsBluetoothActive;
extern bool handshakeCapture;
extern bool aggressiveHandshake;
extern unsigned long handshakeStartTime;
extern bool handshakeComplete;
extern String capturedHandshakes;
extern uint32_t handshakeCount;
extern volatile bool beaconSpamActive;
extern String validPass;
extern unsigned long lastAttackTime;
extern unsigned long lastBeaconTime;
extern uint32_t deauthPkts;
extern uint16_t seqNum;
extern std::vector<Station> stations;
extern uint8_t broadcast[6];
extern char emptySSID[32];
extern uint8_t channelIndex;
extern uint8_t macAddr[6];
extern uint8_t wifi_channel;
extern uint32_t packetSize;
extern uint32_t packetCounter;
extern DNSServer dnsServer;
extern WebServer webServer;
extern TaskHandle_t deauthTaskHandle;
extern bool printerAttackActive;
extern String discoveredPrinters;
extern String printerMessage;

// AP Settings Functions
void loadAPSettings();
void saveAPSettings();

#endif
