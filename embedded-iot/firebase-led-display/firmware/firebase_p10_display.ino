#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const char* FIREBASE_HOST =
  "futmx-led-display-default-rtdb.firebaseio.com";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println(" FIREBASE HTTPS DIAGNOSTIC");
  Serial.println("================================");

  // -------------------------------
  // Wi-Fi
  // -------------------------------
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  // -------------------------------
  // DNS test
  // -------------------------------
  Serial.println();
  Serial.println("Testing DNS...");

  IPAddress serverIP;

  if (WiFi.hostByName(FIREBASE_HOST, serverIP)) {
    Serial.print("Firebase IP: ");
    Serial.println(serverIP);
  } else {
    Serial.println("DNS FAILED!");
    return;
  }

  // -------------------------------
  // HTTPS port test
  // -------------------------------
  Serial.println();
  Serial.println("Testing HTTPS port 443...");

  WiFiClientSecure client;
  client.setInsecure();

  Serial.print("Connecting to ");
  Serial.print(FIREBASE_HOST);
  Serial.println(":443");

  if (client.connect(FIREBASE_HOST, 443)) {
    Serial.println("HTTPS PORT 443: CONNECTED!");

    // Send a very simple HTTPS request
    client.println("GET / HTTP/1.1");
    client.print("Host: ");
    client.println(FIREBASE_HOST);
    client.println("Connection: close");
    client.println();

    Serial.println();
    Serial.println("Server response:");

    unsigned long timeout = millis();

    while (client.connected() && millis() - timeout < 10000) {
      while (client.available()) {
        Serial.write(client.read());
        timeout = millis();
      }
    }

    client.stop();

  } else {
    Serial.println("HTTPS PORT 443: CONNECTION FAILED!");
  }

  Serial.println();
  Serial.println("================================");
  Serial.println(" DIAGNOSTIC COMPLETE");
  Serial.println("================================");
}

void loop() {
}
