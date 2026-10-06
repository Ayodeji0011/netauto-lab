#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// ===================================================================
// 1. HARDWARE PIN CONFIGURATION (UPDATED)
// ===================================================================
#define LED_RED     0   // PRIORITY 1: Emergency Active / Forwarding Error
#define LED_YELLOW  4   // PRIORITY 2: Payload Decryption & Processing Status
#define LED_GREEN   5   // PRIORITY 3: Power / Relay Standby Readiness (Always ON)
#define BUZZER_PIN  19  // Audio Relay Feedback

// ===================================================================
// 2. NETWORK & PEER MAC ADDRESSES
// ===================================================================
// Target Base Station Node 3 (Replace with actual MAC if needed)
uint8_t node3BaseMAC[] = {0xEC, 0xE3, 0x34, 0x9A, 0x80, 0xD0}; 

const char XOR_KEY = 0x5A; // Encryption key matching Node 1 & Node 3

typedef struct struct_message {
  uint8_t msg_id;
  char sender_id[8];
  char payload[64];
  float latitude;
  float longitude;
  uint8_t route_flag; // 0x01 = Primary Relay Route, 0x02 = Direct Failover Route
} struct_message;

struct_message incomingPacket;
struct_message relayPacket;
esp_now_peer_info_t peerBase;

volatile bool packetReceived = false;
volatile bool lastTxSuccess = false;
volatile bool ackWaiting = false;

// ===================================================================
// 3. HELPER FUNCTIONS & LED INDICATION
// ===================================================================
void xorEncryptDecrypt(char *data, size_t len, char key) {
  for (size_t i = 0; i < len && data[i] != '\0'; i++) {
    data[i] ^= key;
  }
}

// Visual Alert on Relay Pulse
void pulseRelayAlert(bool isTestMsg) {
  digitalWrite(LED_GREEN, LOW); // Pause standby LED
  
  if (!isTestMsg) {
    // Emergency Relay Signal: Flash RED LED (GPIO 0) & brief audio burst
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    // Test Signal Relay: Flash YELLOW LED (GPIO 4)
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  digitalWrite(LED_GREEN, HIGH); // Restore standby status LED
}

// ===================================================================
// 4. ESP-NOW CALLBACK HANDLERS
// ===================================================================

// Transmission Callback to Node 3
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 1, 0)
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
#else
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
#endif
  ackWaiting = false;
  lastTxSuccess = (status == ESP_NOW_SEND_SUCCESS);
}

// Receive Callback from Node 1
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 1, 0)
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
#else
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
#endif
  if (len == sizeof(struct_message)) {
    memcpy(&incomingPacket, incomingData, sizeof(incomingPacket));
    packetReceived = true;
  }
}

// ===================================================================
// 5. RELAY FORWARDING LOGIC
// ===================================================================
void processAndRelayPacket() {
  digitalWrite(LED_YELLOW, HIGH); // Indicate Active Data Processing

  // 1. Duplicate Packet to Relay Buffer
  memcpy(&relayPacket, &incomingPacket, sizeof(struct_message));

  // 2. Temporarily Decrypt Payload for Local Verification Logging
  char tempBuffer[64];
  memset(tempBuffer, 0, sizeof(tempBuffer));
  memcpy(tempBuffer, relayPacket.payload, sizeof(relayPacket.payload));
  xorEncryptDecrypt(tempBuffer, strlen(tempBuffer), XOR_KEY);

  bool isTest = (strcmp(tempBuffer, "SYSTEM TEST") == 0 || strstr(tempBuffer, "PING") != NULL);

  Serial.println("\n-------------------------------------------");
  Serial.println("📥 [NODE 2 RELAY] Packet Received!");
  Serial.print("   • Msg ID: "); Serial.println(relayPacket.msg_id);
  Serial.print("   • Original Sender: "); Serial.println(relayPacket.sender_id);
  Serial.print("   • Decrypted Payload Peek: "); Serial.println(tempBuffer);
  Serial.print("   • GPS Coordinates: "); 
  Serial.print(relayPacket.latitude, 6); Serial.print(", ");
  Serial.println(relayPacket.longitude, 6);

  // 3. Trigger Relay LED Feedback
  digitalWrite(LED_YELLOW, LOW);
  pulseRelayAlert(isTest);

  // 4. Relay Forward to Node 3 (Base Station)
  Serial.println("📡 Forwarding Packet to Node 3 (Base Station)...");
  digitalWrite(LED_YELLOW, HIGH);

  ackWaiting = true;
  lastTxSuccess = false;

  esp_err_t result = esp_now_send(node3BaseMAC, (uint8_t *)&relayPacket, sizeof(relayPacket));

  int timeout = 0;
  while (ackWaiting && timeout < 20) {
    delay(10);
    timeout++;
  }

  digitalWrite(LED_YELLOW, LOW);

  if (result == ESP_OK && lastTxSuccess) {
    Serial.println("✅ [RELAY SUCCESS] ACK Received from Node 3 Base Station.");
  } else {
    Serial.println("❌ [RELAY ERROR] Node 3 Base Station Unreachable!");
    
    // Solid Error Indication on Relay Failure
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(600);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
  Serial.println("-------------------------------------------\n");
}

// ===================================================================
// 6. SETUP & LOOP
// ===================================================================
void setup() {
  Serial.begin(115200);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  // GREEN LED (GPIO 5) PERMANENTLY ON: Power & Relay Active Indicator
  digitalWrite(LED_GREEN, HIGH);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW Initialization Failed!");
    digitalWrite(LED_RED, HIGH);
    return;
  }

  // Register Callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register Node 3 Base Station Peer
  memcpy(peerBase.peer_addr, node3BaseMAC, 6);
  peerBase.channel = 1;
  peerBase.encrypt = false;
  esp_now_add_peer(&peerBase);

  Serial.println("==================================================");
  Serial.println("  [NODE 2 RELAY NODE] ONLINE");
  Serial.println("  PINS: RED=GPIO0, YELLOW=GPIO4, GREEN=GPIO5, BUZZER=GPIO19");
  Serial.println("  Listening for Node 1 Signals on Channel 1...");
  Serial.println("==================================================");
}

void loop() {
  if (packetReceived) {
    packetReceived = false;
    processAndRelayPacket();
  }
  delay(10);
}
