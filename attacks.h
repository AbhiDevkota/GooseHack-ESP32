#ifndef ATTACKS_H
#define ATTACKS_H

#include "config.h"

// LED control functions
void initLEDs();
void updateLEDs();

NimBLEAdvertisementData getSourAppleData();
void startSourApple();
void stopSourApple();
IRAM_ATTR void sniffer(void* buf, wifi_promiscuous_pkt_type_t type);
void sendDeauthFrame(const uint8_t* destMac, const uint8_t* srcMac, uint8_t frameType);
void sendDeauthPackets();
void nextChannel();
void randomMac();
void startBeaconSpam();
void stopBeaconSpam();
void sendBeaconSpam();
String evilCheckPassword(const String& pass);
void startAttack();
void stopAttack();
void startEvil();
void stopEvil();
void deauthTask(void* param);
void startCombined();
void stopCombined();
void startWindowsBluetooth();
void stopWindowsBluetooth();
NimBLEAdvertisementData getWindowsBluetoothData();
void startHandshakeCapture();
void stopHandshakeCapture();
void startAggressiveHandshake();
void stopAggressiveHandshake();
void clearHandshakes();
IRAM_ATTR void handshakeSniffer(void* buf, wifi_promiscuous_pkt_type_t type);
String generatePCAP();
bool isHandshakePacket(const uint8_t* data, uint16_t len);
void scanNetworkPrinters();
bool connectToNetwork(String ssid, String password);
void printToPrinter(String printerIP, String message);
void startPrinterAttack();
void stopPrinterAttack();

#endif
