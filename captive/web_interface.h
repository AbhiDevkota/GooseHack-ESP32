#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include "config.h"

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

#include "web_styles.h"

String input(const char* argName);
String htmlHeader(String title);
String htmlFooter();
String getEncryptionType(uint8_t type);
String macToString(const uint8_t* mac);
String controlPanel();
String evilIndex(String error = "");
String viewPasswordPage();
void setupWebServer();

#endif
