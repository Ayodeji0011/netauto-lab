#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WebServer.h>
#include <TinyGPS++.h>
#include <Keypad.h>

#define BUZZER_PIN 19
#define LED_GREEN 18
#define LED_YELLOW 4
#define LED_RED 5

// GPS Serial Pins
#define RXD2 21
#define TXD2 22

TinyGPSPlus gps;
HardwareSerial neogps(2);
WebServer server(80);

// Node MAC Addresses
uint8_t macNode2[] = {0x78, 0x1C, 0x3C, 0x2D, 0xA0, 0x8C};
uint8_t macNode3[] = {0xEC, 0xE3, 0x34, 0x9A, 0x80, 0xD0}; // Node 3 MAC

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

typedef struct struct_message {
  uint8_t senderID;
  uint32_t packetID;
  uint8_t emergencyType;
  char messageText[64];
  double latitude;
  double longitude;
  bool isRelayed;
} struct_message;

typedef struct struct_ack {
  uint8_t responderID;
  uint32_t packetID;
  bool acknowledged;
} struct_ack;

struct_message myData;
struct_ack incomingACK;

uint32_t packetCounter = 0;
unsigned long buzzerTimer = 0;
bool buzzerActive = false;

const char* emergencyList[] = {
  "MEDICAL EMERGENCY",
  "FIRE ALERT",
  "SECURITY BREACH",
  "SYSTEM FAILURE",
  "DISASTER EVACUATION",
  "HAZARD WARNING",
  "AMBULANCE NEEDED",
  "GENERAL DISTRESS"
};

void addPeer(uint8_t *mac) {
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, mac, 6);
  peer.channel = 1;
  peer.encrypt = false;
  esp_now_add_peer(&peer);
}

void triggerBeep(unsigned long durationMs) {
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_RED, HIGH);
  buzzerTimer = millis() + durationMs;
  buzzerActive = true;
}

void checkBuzzer() {
  if (buzzerActive && millis() >= buzzerTimer) {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    buzzerActive = false;
  }
}

void sendEmergency(uint8_t typeCode, const char* customText = nullptr, int targetNode = 0) {
  // targetNode: 0 = Both (System Relay), 2 = Node 2 Direct, 3 = Node 3 Direct
  packetCounter++;
  myData.senderID = 1;
  myData.packetID = packetCounter;
  myData.emergencyType = typeCode;
  myData.isRelayed = false;

  String finalMsg = "";
  if (targetNode == 2) {
    finalMsg = "NODE2: ";
  }

  if (customText != nullptr && strlen(customText) > 0) {
    finalMsg += String(customText);
  } else if (typeCode >= 1 && typeCode <= 8) {
    finalMsg += String(emergencyList[typeCode - 1]);
  } else {
    finalMsg += "CUSTOM ALERT";
  }

  strncpy(myData.messageText, finalMsg.c_str(), sizeof(myData.messageText) - 1);
  myData.messageText[sizeof(myData.messageText) - 1] = '\0';

  if (gps.location.isValid()) {
    myData.latitude = gps.location.lat();
    myData.longitude = gps.location.lng();
  } else {
    myData.latitude = 0.0;
    myData.longitude = 0.0;
  }

  if (targetNode == 2) {
    // Direct Transmission to Node 2 Only
    esp_now_send(macNode2, (uint8_t *)&myData, sizeof(myData));
    Serial.printf("[TX NODE 1 -> NODE 2] Direct Alert #%d: %s\n", myData.packetID, myData.messageText);
  } else if (targetNode == 3) {
    // Direct Transmission to Node 3 Only
    esp_now_send(macNode3, (uint8_t *)&myData, sizeof(myData));
    Serial.printf("[TX NODE 1 -> NODE 3] Direct Alert #%d: %s\n", myData.packetID, myData.messageText);
  } else {
    // Broadcast / Relay Mode (Send to both Node 2 and Node 3)
    esp_now_send(macNode2, (uint8_t *)&myData, sizeof(myData));
    esp_now_send(macNode3, (uint8_t *)&myData, sizeof(myData));
    Serial.printf("[TX NODE 1 -> ALL] Broadcast Alert #%d: %s\n", myData.packetID, myData.messageText);
  }

  triggerBeep(2000);
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Node 1 - Transmitter</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:Arial;background:#121212;color:#fff;text-align:center;padding:15px;}";
  html += ".card{background:#1e1e1e;border-radius:10px;padding:15px;margin:15px auto;max-width:400px;}";
  html += ".btn{background:#d32f2f;color:white;border:none;padding:12px;margin:5px 0;font-size:15px;border-radius:5px;width:100%;cursor:pointer;}";
  html += ".custom-btn{background:#2196F3;}";
  html += "input[type='text'], select{width:92%;padding:10px;margin:10px 0;border-radius:5px;border:1px solid #444;background:#2a2a2a;color:#fff;font-size:14px;}";
  html += "</style><meta http-equiv='refresh' content='5'></head><body>";

  html += "<h2>NODE 1: TRANSMITTER DASHBOARD</h2>";

  // GPS Telemetry Card
  html += "<div class='card'><h3>GPS TELEMETRY</h3>";
  if (gps.location.isValid()) {
    html += "<p><strong>Status:</strong> <span style='color:#4CAF50;'>LOCKED</span></p>";
    html += "<p><strong>Lat/Lng:</strong> " + String(gps.location.lat(), 6) + ", " + String(gps.location.lng(), 6) + "</p>";
    html += "<p><strong>Satellites:</strong> " + String(gps.satellites.value()) + "</p>";
  } else {
    html += "<p><strong>Status:</strong> <span style='color:#ff9800;'>SEARCHING SATELLITES...</span></p>";
    html += "<p><strong>Satellites Tracked:</strong> " + String(gps.satellites.value()) + "</p>";
  }
  html += "<p><strong>Last Packet Sent:</strong> #" + String(packetCounter) + "</p></div>";

  // Custom Message & Target Selection Card
  html += "<div class='card'><h3>SEND CUSTOM MESSAGE</h3>";
  html += "<form action='/send_custom' method='POST'>";
  html += "<label>Select Destination:</label><br>";
  html += "<select name='target'>";
  html += "<option value='0'>Both / System Relay (Node 2 & Node 3)</option>";
  html += "<option value='2'>Node 2 (Relay Node Only)</option>";
  html += "<option value='3'>Node 3 (Receiver Node Only)</option>";
  html += "</select><br>";
  html += "<input type='text' name='msg' placeholder='Type emergency message...' maxlength='50' required>";
  html += "<button class='btn custom-btn' type='submit'>SEND CUSTOM TEXT ALERT</button>";
  html += "</form></div>";

  // Preset Buttons Card
  html += "<div class='card'><h3>PRESET ALERTS (ALL NODES)</h3>";
  for (int i = 0; i < 8; i++) {
    html += "<form action='/send' method='GET'><input type='hidden' name='code' value='" + String(i + 1) + "'>";
    html += "<button class='btn'>" + String(i + 1) + ". " + String(emergencyList[i]) + "</button></form>";
  }
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

void handleSend() {
  if (server.hasArg("code")) {
    int code = server.arg("code").toInt();
    sendEmergency(code, nullptr, 0);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleSendCustom() {
  if (server.hasArg("msg")) {
    String text = server.arg("msg");
    int target = server.hasArg("target") ? server.arg("target").toInt() : 0;
    sendEmergency(99, text.c_str(), target);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
  if (len == sizeof(struct_ack)) {
    memcpy(&incomingACK, data, sizeof(incomingACK));
    if (incomingACK.acknowledged) {
      Serial.printf("[ACK RECEIVED] Node %d Confirmed Packet #%d\n", incomingACK.responderID, incomingACK.packetID);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      buzzerActive = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Emergency-Node1", "123456789", 1);

  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  if (esp_now_init() == ESP_OK) {
    esp_now_register_recv_cb(OnDataRecv);
    addPeer(macNode2);
    addPeer(macNode3);
  }

  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.on("/send_custom", HTTP_POST, handleSendCustom);
  server.begin();

  Serial.println("Node 1 Transmitter Ready");
}

void loop() {
  server.handleClient();
  checkBuzzer();

  while (neogps.available()) {
    gps.encode(neogps.read());
  }

  char key = keypad.getKey();
  if (key >= '1' && key <= '8') {
    sendEmergency(key - '0', nullptr, 0);
  }
}
