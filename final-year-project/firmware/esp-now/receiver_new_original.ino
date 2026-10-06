#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define NODE_ID "BASE_RECEIVER_03"

// Static Network Routing Layout - Matches Node 2 perfectly
uint8_t NODE3_BASE[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x03}; 

// Emergency Unified Data Structure Payload (Must match Sender and Relay exactly)
struct __attribute__((packed)) EmergencyPacket {
    uint8_t nodeId;        // 1 = Hostel
    uint8_t alertType;     // 1 = Fire, 2 = Medical, 3 = Security Threat, 4 = Custom/Panic
    uint32_t timestamp;    
    float latitude;        
    float longitude;       
    uint8_t hopCount;      // Multi-hop path counter
};

const char CRYPTO_KEY = 'Y'; 

// Initialize the 128x64 OLED Display
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// Updated hardware pin layout to avoid boot restrictions entirely
#define RED_LED    25
#define YELLOW_LED 26
#define GREEN_LED  12   // Moved LED here (Safe output pin)
#define BUZZER     4

// Fast XOR Decryption Routine
void maskPayload(uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        data[i] ^= CRYPTO_KEY;
    }
}

// Convert alert integers into readable string titles
String getAlertName(uint8_t type) {
    switch(type) {
        case 1:  return "FIRE EMERGENCY";
        case 2:  return "MEDICAL ALARM";
        case 3:  return "SECURITY THREAT";
        case 4:  return "GENERAL PANIC";
        default: return "UNKNOWN CRISIS";
    }
}

// Loud pulsing alarm profile for the base command station
void triggerBaseAlarm() {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    
    for(int i = 0; i < 6; i++) {
        digitalWrite(BUZZER, HIGH); delay(100);
        digitalWrite(BUZZER, LOW);  delay(80);
    }
}

// Execution callback fired when the Relay node routes a packet here
void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
    EmergencyPacket alertPacket;
    
    if (len == sizeof(alertPacket)) {
        memcpy(&alertPacket, incomingData, sizeof(alertPacket));
        
        // Decrypt incoming metrics from the radio wave space
        maskPayload((uint8_t*)&alertPacket, sizeof(alertPacket));
        
        Serial.println("\n--- BASE COMMAND CENTER: ALERT RECEIVED! ---");
        
        // Fire indicators immediately
        triggerBaseAlarm();

        // Render data seamlessly to the OLED Display screen interface
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        
        display.setCursor(0, 0);
        display.println("!! DISTRESS ALERT !!");
        display.println("--------------------");
        
        display.print("SRC STN: NODE 0"); display.println(alertPacket.nodeId);
        display.print("TYPE: "); display.println(getAlertName(alertPacket.alertType));
        display.print("ROUTED HOPS: "); display.println(alertPacket.hopCount);
        
        display.print("LAT: "); display.println(alertPacket.latitude, 5);
        display.print("LNG: "); display.println(alertPacket.longitude, 5);
        display.display();

        // Echo an execution confirmation back to Node 2 link layer
        uint8_t backAck[1] = {'A'};
        esp_now_send(info->src_addr, backAck, 1);
        
        // Keep alert displayed layout frozen for 5 seconds before going green again
        delay(5000); 
        
        // Reset base board to idle/listening state
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("BASE TERMINAL SECURE");
        display.println("STATUS: LISTENING...");
        display.display();
    }
}

void setup() {
    Serial.begin(115200);
    
    pinMode(RED_LED, OUTPUT); pinMode(YELLOW_LED, OUTPUT); pinMode(GREEN_LED, OUTPUT); pinMode(BUZZER, OUTPUT);
    digitalWrite(GREEN_LED, HIGH); 

    // Fire up your clean, safe I2C bus wiring (SDA = 27, SCL = 13)
    Wire.begin(27, 13);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED allocation failed!");
        for(;;);
    }
    
    // Initializing splash text display profile
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println("BASE TERMINAL SECURE");
    display.println("STATUS: LISTENING...");
    display.display();

    WiFi.mode(WIFI_STA);
    esp_wifi_set_mac(WIFI_IF_STA, NODE3_BASE);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        return;
    }
    
    esp_now_register_recv_cb(onDataRecv);
    
    // Bind Relay Node 2 back as a communication partner link to acknowledge routed signals
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, NODE3_BASE, 6); // Uses Relay mapping loop directly
    peerInfo.channel = 1;  
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);

    Serial.println("=== SYSTEM ONLINE: MONITORING BASE READY ===");
}

void loop() {
    delay(20); // Thread control stabilization loop
}
