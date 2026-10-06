#include "web_server_handler.h"
#include "tts_handler.h"
#include "web_ui.h"
#include <ArduinoJson.h>

WebServer server(80);


// =====================================================
// CORS — required so a page saved locally (file://) is
// allowed to fetch() this board across origins.
// =====================================================
void sendCORSHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void handleOptions() {
  sendCORSHeaders();
  server.send(204);
}


// =====================================================
// ROOT PAGE
// =====================================================
void handleRoot() {
  sendCORSHeaders();
  server.send_P(200, "text/html", INDEX_HTML);
}


// =====================================================
// DISPLAY MESSAGE  (TTT + STT both land here)
// =====================================================
void handleDisplay() {

  sendCORSHeaders();

  if (server.method() != HTTP_POST) {
    server.send(405, "application/json", "{\"status\":\"error\",\"message\":\"POST required\"}");
    return;
  }

  String body = server.arg("plain");

  StaticJsonDocument<512> doc;
  DeserializationError err = deserializeJson(doc, body);

  if (err) {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
    return;
  }

  String line1 = doc["line1"] | "";
  String line2 = doc["line2"] | "";

  line1.trim();
  line2.trim();

  if (xSemaphoreTake(displayMutex, portMAX_DELAY)) {
    sharedDisplay.line1 = line1;
    sharedDisplay.line2 = line2;
    newMessageFlag = true;
    xSemaphoreGive(displayMutex);
  }

  server.send(200, "application/json", "{\"status\":\"success\"}");
}


// =====================================================
// AUDIO  (TTS tab — canned clip playback)
// =====================================================
void handleAudio() {

  sendCORSHeaders();

  if (server.method() != HTTP_POST) {
    server.send(405, "application/json", "{\"status\":\"error\",\"message\":\"POST required\"}");
    return;
  }

  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));

  if (err) {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
    return;
  }

  String action = doc["action"] | "play";

  if (action == "stop") {
    stopAudio();
  } else {
    String name   = doc["name"]   | "";
    bool   repeat = doc["repeat"] | false;

    if (name == "") {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing name\"}");
      return;
    }

    playAudio(name, repeat);
  }

  server.send(200, "application/json", "{\"status\":\"success\"}");
}


// =====================================================
// STATUS
// =====================================================
void handleStatus() {
  sendCORSHeaders();
  server.send(200, "application/json", "{\"status\":\"online\"}");
}


// =====================================================
// WEB SERVER INITIALIZATION
// =====================================================
void initWebServer() {

  server.on("/",        HTTP_GET,     handleRoot);
  server.on("/display", HTTP_POST,    handleDisplay);
  server.on("/display", HTTP_OPTIONS, handleOptions);
  server.on("/audio",   HTTP_POST,    handleAudio);
  server.on("/audio",   HTTP_OPTIONS, handleOptions);
  server.on("/status",  HTTP_GET,     handleStatus);

  server.begin();
  Serial.println("Web server started");

  // Bring up the DFPlayer here so the .ino doesn't need editing.
  initDFPlayer();
}


// =====================================================
// WEB SERVER TASK
// =====================================================
void webServerTask(void *pvParameters) {

  while (true) {
    server.handleClient();
    checkAudioRepeat();
    vTaskDelay(pdMS_TO_TICKS(2));
  }
}
