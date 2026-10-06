#include <Arduino.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "shared_data.h"
#include "web_server_handler.h"
#include "display_handler.h"

// SoftAP credentials
#define AP_SSID     "DisplayBoard"     // WiFi name phone will see
#define AP_PASSWORD "YOUR_AP_PASSWORD"      // WiFi password
#define AP_CHANNEL  1
#define AP_MAX_CONN 1                  // max devices that can connect

volatile bool newMessageFlag = false;
DisplayData sharedDisplay;
SemaphoreHandle_t displayMutex;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);

  // Start the SoftAP Wi-Fi network for your phone app
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD, AP_CHANNEL, 0, AP_MAX_CONN);

  IPAddress apIP = WiFi.softAPIP();
  Serial.println("SoftAP started!");
  Serial.println("Network name: " + String(AP_SSID));
  Serial.println("ESP32 IP: " + apIP.toString());

  // Initialize shared memory mutext and setup your custom matrix engine pins
  displayMutex = xSemaphoreCreateMutex();
  initWebServer();
  initDisplay(); 

  // Launch your tasks across the two processor cores
  // Core 0 handles the phone app network; Core 1 handles your fast bit-shifting display loop
  xTaskCreatePinnedToCore(webServerTask, "WebServerTask", 8192,  NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(displayTask,   "DisplayTask",   12288, NULL, 1, NULL, 1);
}

void loop() {
  // FreeRTOS tasks take over, leaving the main loop empty
}
