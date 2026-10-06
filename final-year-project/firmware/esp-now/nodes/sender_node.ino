#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// ============================================================
// NODE 2 - RELAY / DIRECT ALERT NODE
// ============================================================

#define NODE_ID "NODE_2"

// ============================================================
// REAL MAC ADDRESS OF NODE 3
// ============================================================

uint8_t node3MAC[] = {
  0xEC, 0xE3, 0x34, 0xDB, 0x51, 0xEC
};

// ============================================================
// NODE 2 PINS
// ============================================================

#define LED_RED     0
#define LED_YELLOW  4
#define LED_GREEN   5

#define BUZZER_PIN  19

// ============================================================
// XOR KEY
// ============================================================

const char XOR_KEY = 0x5A;

// ============================================================
// DESTINATIONS
// ============================================================

#define DEST_NODE_2 2
#define DEST_NODE_3 3

// ============================================================
// PACKET STRUCTURE
// MUST MATCH NODE 1 EXACTLY
// ============================================================

typedef struct {

  uint8_t msg_id;

  char sender_id[16];

  char payload[64];

  float latitude;

  float longitude;

  bool gpsValid;

  uint8_t destination;

} struct_message;

struct_message incomingPacket;
struct_message relayPacket;

volatile bool packetReceived = false;

volatile bool transmissionDone = false;
volatile bool transmissionSuccess = false;

// ============================================================
// ALERT STATE
// ============================================================

bool directAlertActive = false;

// ============================================================
// XOR FUNCTION
// ============================================================

void xorEncryptDecrypt(
  char *data,
  size_t len
) {

  for (
    size_t i = 0;
    i < len;
    i++
  ) {

    data[i] ^= XOR_KEY;
  }
}

// ============================================================
// SYSTEM READY
// ============================================================

void systemReady() {

  digitalWrite(
    LED_RED,
    LOW
  );

  digitalWrite(
    LED_YELLOW,
    LOW
  );

  digitalWrite(
    LED_GREEN,
    HIGH
  );
}

// ============================================================
// ERROR ALERT
// ============================================================

void errorAlert() {

  digitalWrite(
    LED_GREEN,
    LOW
  );

  for (
    int i = 0;
    i < 5;
    i++
  ) {

    digitalWrite(
      LED_RED,
      HIGH
    );

    digitalWrite(
      BUZZER_PIN,
      HIGH
    );

    delay(400);

    digitalWrite(
      LED_RED,
      LOW
    );

    digitalWrite(
      BUZZER_PIN,
      LOW
    );

    delay(300);
  }

  systemReady();
}

// ============================================================
// ESP-NOW SEND CALLBACK
// ============================================================

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 1, 0)

void OnDataSent(
  const wifi_tx_info_t *info,
  esp_now_send_status_t status
)

#else

void OnDataSent(
  const uint8_t *mac_addr,
  esp_now_send_status_t status
)

#endif
{

  transmissionDone = true;

  transmissionSuccess =
    (status == ESP_NOW_SEND_SUCCESS);
}

// ============================================================
// ESP-NOW RECEIVE CALLBACK
// ============================================================

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 1, 0)

void OnDataRecv(

  const esp_now_recv_info_t *recv_info,

  const uint8_t *incomingData,

  int len

)

#else

void OnDataRecv(

  const uint8_t *mac,

  const uint8_t *incomingData,

  int len

)

#endif
{

  if (
    len == sizeof(struct_message)
  ) {

    memcpy(

      &incomingPacket,

      incomingData,

      sizeof(incomingPacket)

    );

    packetReceived = true;
  }
}

// ============================================================
// DIRECT NODE 2 ALERT
// ============================================================

void activateDirectAlert() {

  directAlertActive = true;

  Serial.println();
  Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  Serial.println("DIRECT MESSAGE FOR NODE 2");
  Serial.println("NOT FORWARDING TO NODE 3");
  Serial.println("BUZZER ACTIVE");
  Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}

// ============================================================
// PROCESS PACKET
// ============================================================

void processPacket() {

  Serial.println();
  Serial.println("================================");
  Serial.println("NODE 2 RECEIVED MESSAGE");
  Serial.println("================================");

  memcpy(

    &relayPacket,

    &incomingPacket,

    sizeof(struct_message)

  );

  // Temporary decrypted copy

  char decryptedPayload[64];

  memset(
    decryptedPayload,
    0,
    sizeof(decryptedPayload)
  );

  memcpy(

    decryptedPayload,

    relayPacket.payload,

    sizeof(relayPacket.payload)

  );

  xorEncryptDecrypt(

    decryptedPayload,

    strlen(decryptedPayload)

  );

  Serial.print("FROM: ");

  Serial.println(
    relayPacket.sender_id
  );

  Serial.print("MESSAGE: ");

  Serial.println(
    decryptedPayload
  );

  Serial.print("DESTINATION: ");

  Serial.println(
    relayPacket.destination
  );

  // ========================================================
  // MESSAGE IS FOR NODE 2 ONLY
  // ========================================================

  if (
    relayPacket.destination ==
    DEST_NODE_2
  ) {

    activateDirectAlert();

    return;
  }

  // ========================================================
  // MESSAGE IS FOR NODE 3
  // ========================================================

  if (
    relayPacket.destination ==
    DEST_NODE_3
  ) {

    Serial.println(
      "FORWARDING MESSAGE TO NODE 3"
    );

    digitalWrite(
      LED_GREEN,
      LOW
    );

    digitalWrite(
      LED_YELLOW,
      HIGH
    );

    transmissionDone = false;

    transmissionSuccess = false;

    esp_err_t result =

      esp_now_send(

        node3MAC,

        (uint8_t *)&relayPacket,

        sizeof(relayPacket)

      );


    if (
      result != ESP_OK
    ) {

      Serial.println(
        "FAILED TO START TRANSMISSION"
      );

      digitalWrite(
        LED_YELLOW,
        LOW
      );

      errorAlert();

      return;
    }


    unsigned long startTime =
      millis();


    while (

      !transmissionDone &&

      millis() - startTime < 5000

    ) {

      delay(10);
    }


    digitalWrite(
      LED_YELLOW,
      LOW
    );


    if (

      transmissionDone &&

      transmissionSuccess

    ) {

      Serial.println(
        "SUCCESSFULLY FORWARDED TO NODE 3"
      );

      digitalWrite(
        LED_GREEN,
        HIGH
      );

      digitalWrite(
        BUZZER_PIN,
        HIGH
      );

      delay(250);

      digitalWrite(
        BUZZER_PIN,
        LOW
      );

    }

    else {

      Serial.println(
        "NODE 3 UNREACHABLE"
      );

      errorAlert();
    }
  }
}

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  pinMode(
    LED_RED,
    OUTPUT
  );

  pinMode(
    LED_YELLOW,
    OUTPUT
  );

  pinMode(
    LED_GREEN,
    OUTPUT
  );

  pinMode(
    BUZZER_PIN,
    OUTPUT
  );

  digitalWrite(
    BUZZER_PIN,
    LOW
  );

  systemReady();

  // WiFi Station

  WiFi.mode(
    WIFI_STA
  );

  // ESP-NOW CHANNEL 1

  esp_wifi_set_channel(

    1,

    WIFI_SECOND_CHAN_NONE

  );

  Serial.print(
    "NODE 2 MAC: "
  );

  Serial.println(
    WiFi.macAddress()
  );

  // Initialize ESP-NOW

  if (

    esp_now_init()

    != ESP_OK

  ) {

    Serial.println(
      "ESP-NOW INIT FAILED"
    );

    digitalWrite(
      LED_RED,
      HIGH
    );

    return;
  }

  // Callbacks

  esp_now_register_recv_cb(
    OnDataRecv
  );

  esp_now_register_send_cb(
    OnDataSent
  );

  // Add Node 3

  esp_now_peer_info_t peerInfo = {};

  memcpy(

    peerInfo.peer_addr,

    node3MAC,

    6

  );

  peerInfo.channel = 1;

  peerInfo.encrypt = false;

  if (

    esp_now_add_peer(
      &peerInfo
    )

    != ESP_OK

  ) {

    Serial.println(
      "FAILED TO ADD NODE 3"
    );

    digitalWrite(
      LED_RED,
      HIGH
    );

    return;
  }

  Serial.println();
  Serial.println("================================");
  Serial.println("NODE 2 READY");
  Serial.println("LISTENING FOR NODE 1");
  Serial.println("CHANNEL: 1");
  Serial.println("================================");
}

// ============================================================
// LOOP
// ============================================================

void loop() {

  // --------------------------------------------------------
  // CONTINUOUS DIRECT ALERT
  // --------------------------------------------------------

  if (
    directAlertActive
  ) {

    digitalWrite(
      LED_RED,
      HIGH
    );

    digitalWrite(
      LED_GREEN,
      LOW
    );

    digitalWrite(
      LED_YELLOW,
      LOW
    );

    // Continuous beep pattern
    digitalWrite(
      BUZZER_PIN,
      HIGH
    );

    delay(500);

    digitalWrite(
      BUZZER_PIN,
      LOW
    );

    delay(300);

    return;
  }

  // --------------------------------------------------------
  // NORMAL PACKET PROCESSING
  // --------------------------------------------------------

  if (
    packetReceived
  ) {

    packetReceived = false;

    processPacket();
  }

  delay(10);
}
