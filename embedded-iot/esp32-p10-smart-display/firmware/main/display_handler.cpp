#include "display_handler.h"

// ============================================================
// P10 PIN CONFIGURATION
// ============================================================

#define DATA_PIN 23
#define CLK_PIN  18
#define LAT_PIN  2
#define OE_PIN   22
#define A_PIN    19
#define B_PIN    21

// ============================================================
// DISPLAY CONFIGURATION
// ============================================================

#define WIDTH        96
#define HEIGHT       32
#define STRIDE       12
#define BUFFER_SIZE  384

// ============================================================
// DOUBLE FRAMEBUFFER
// ============================================================

uint8_t framebufferA[BUFFER_SIZE];
uint8_t framebufferB[BUFFER_SIZE];

uint8_t *activeBuffer = framebufferA;
uint8_t *drawBuffer   = framebufferB;

// ============================================================
// 5 x 7 FONT
// ============================================================

const uint8_t ASCII_FONT[95][7] = {

  {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x04,0x04,0x04,0x04,0x00,0x00,0x04},
  {0x0A,0x0A,0x0A,0x00,0x00,0x00,0x00},
  {0x0A,0x0A,0x1F,0x0A,0x1F,0x0A,0x0A},
  {0x04,0x1E,0x05,0x0E,0x14,0x0F,0x04},
  {0x13,0x13,0x08,0x04,0x02,0x19,0x19},
  {0x0C,0x12,0x14,0x08,0x15,0x12,0x0D},
  {0x06,0x04,0x02,0x00,0x00,0x00,0x00},
  {0x02,0x04,0x08,0x08,0x08,0x04,0x02},
  {0x08,0x04,0x02,0x02,0x02,0x04,0x08},
  {0x04,0x15,0x0E,0x04,0x0E,0x15,0x04},
  {0x04,0x04,0x1F,0x04,0x04,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x06,0x04,0x02},
  {0x00,0x00,0x1F,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C},
  {0x01,0x02,0x04,0x08,0x10,0x20,0x40},

  {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E},
  {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E},
  {0x0E,0x11,0x01,0x02,0x04,0x08,0x1F},
  {0x1F,0x02,0x04,0x02,0x01,0x11,0x0E},
  {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02},
  {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E},
  {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E},
  {0x1F,0x01,0x02,0x04,0x08,0x08,0x08},
  {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E},
  {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C},

  {0x00,0x0C,0x0C,0x00,0x0C,0x0C,0x00},
  {0x00,0x0C,0x0C,0x00,0x0C,0x04,0x02},
  {0x02,0x04,0x08,0x10,0x08,0x04,0x02},
  {0x00,0x1F,0x00,0x1F,0x00,0x00,0x00},
  {0x08,0x04,0x02,0x01,0x02,0x04,0x08},
  {0x0E,0x11,0x01,0x02,0x04,0x00,0x04},

  {0x0E,0x11,0x17,0x15,0x17,0x10,0x0F},

  {0x04,0x0A,0x11,0x11,0x1F,0x11,0x11},
  {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E},
  {0x0E,0x11,0x10,0x10,0x10,0x11,0x0E},
  {0x1C,0x12,0x11,0x11,0x11,0x12,0x1C},
  {0x1F,0x10,0x10,0x1F,0x10,0x10,0x1F},
  {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10},
  {0x0E,0x11,0x10,0x17,0x11,0x11,0x0F},
  {0x11,0x11,0x11,0x1F,0x11,0x11,0x11},
  {0x0E,0x04,0x04,0x04,0x04,0x04,0x0E},
  {0x1F,0x02,0x02,0x02,0x02,0x12,0x0C},
  {0x11,0x12,0x14,0x18,0x14,0x12,0x11},
  {0x10,0x10,0x10,0x10,0x10,0x10,0x1F},
  {0x11,0x1B,0x15,0x11,0x11,0x11,0x11},
  {0x11,0x11,0x19,0x15,0x13,0x11,0x11},
  {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E},
  {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10},
  {0x0E,0x11,0x11,0x11,0x15,0x12,0x0D},
  {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11},
  {0x0E,0x11,0x10,0x0E,0x01,0x11,0x0E},
  {0x1F,0x04,0x04,0x04,0x04,0x04,0x04},
  {0x11,0x11,0x11,0x11,0x11,0x11,0x0E},
  {0x11,0x11,0x11,0x11,0x11,0x0A,0x04},
  {0x11,0x11,0x11,0x15,0x15,0x1B,0x11},
  {0x11,0x11,0x0A,0x04,0x0A,0x11,0x11},
  {0x11,0x11,0x0A,0x04,0x04,0x04,0x04},
  {0x1F,0x02,0x04,0x08,0x10,0x20,0x1F},

  {0x0E,0x08,0x08,0x08,0x08,0x08,0x0E},
  {0x10,0x08,0x04,0x02,0x01,0x00,0x00},
  {0x0E,0x02,0x02,0x02,0x02,0x02,0x0E},
  {0x04,0x0A,0x11,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x1F},
  {0x04,0x02,0x01,0x00,0x00,0x00,0x00}
};

// ============================================================
// SET PIXEL
// ============================================================

void setPixel(
  uint8_t *buffer,
  int x,
  int y,
  bool on
) {
  if (x < 0 || x >= WIDTH ||
      y < 0 || y >= HEIGHT) {
    return;
  }

  // Known-good horizontal mirror.
  // DO NOT vertically mirror.
  int targetX = (WIDTH - 1) - x;

  int index =
    y * STRIDE +
    (targetX >> 3);

  uint8_t mask =
    0x80 >> (targetX & 7);

  if (on)
    buffer[index] &= ~mask;
  else
    buffer[index] |= mask;
}

// ============================================================
// CLEAR BUFFER
// ============================================================

void clearBuffer(uint8_t *buffer) {
  memset(buffer, 0xFF, BUFFER_SIZE);
}

// ============================================================
// DRAW CHARACTER
// ============================================================

void drawLetterOnBuffer(
  uint8_t *buffer,
  int startX,
  int startY,
  char c,
  int scale
) {
  if (c >= 'a' && c <= 'z')
    c -= 32;

  int fontIndex = c - 32;

  if (fontIndex < 0 || fontIndex >= 95)
    fontIndex = 0;

  for (int row = 0; row < 7; row++) {

    uint8_t rowData =
      ASCII_FONT[fontIndex][row];

    for (int col = 0; col < 5; col++) {

      // Fixed: Properly check columns 0 through 4 bitwise
      if (rowData & (1 << (4 - col))) {

        for (int sy = 0; sy < scale; sy++) {
          for (int sx = 0; sx < scale; sx++) {

            setPixel(
              buffer,
              startX + col * scale + sx,
              startY + row * scale + sy,
              true
            );
          }
        }
      }
    }
  }
}

// ============================================================
// DRAW STRING
// ============================================================

void drawStringOnBuffer(
  uint8_t *buffer,
  String text,
  int startX,
  int startY,
  int scale
) {
  int currentX = startX;

  // 5 pixels character + 1 pixel spacing
  int charSpacing = 6 * scale;

  for (int i = 0; i < text.length(); i++) {

    drawLetterOnBuffer(
      buffer,
      currentX,
      startY,
      text[i],
      scale
    );

    currentX += charSpacing;
  }
}

// ============================================================
// GET TEXT WIDTH
// ============================================================

int getTextWidth(
  String text,
  int scale
) {
  if (text.length() == 0)
    return 0;

   return ((text.length() * 5) + (text.length() - 1)) * scale;
}

// ============================================================
// BUILD COMPLETE FRAME
// ============================================================

void buildFrame(
  uint8_t *buffer,
  String line1,
  String line2,
  int scrollX,
  int scale
) {
  clearBuffer(buffer);

  // ----------------------------------------------------------
  // TOP LINE
  // ----------------------------------------------------------

  if (line1.length() > 0) {

    int width1 =
      getTextWidth(line1, scale);

    if (width1 <= WIDTH) {

      // Short text = centered
      int x1 =
        (WIDTH - width1) / 2;

      drawStringOnBuffer(
        buffer,
        line1,
        x1,
        1,
        scale
      );

    } else {

      // Long text = one continuous scrolling line
      drawStringOnBuffer(
        buffer,
        line1,
        scrollX,
        1,
        scale
      );
    }
  }

  // ----------------------------------------------------------
  // BOTTOM LINE
  // ----------------------------------------------------------

  if (line2.length() > 0) {

    int width2 =
      getTextWidth(line2, scale);

    if (width2 <= WIDTH) {

      // Short text = centered
      int x2 =
        (WIDTH - width2) / 2;

      drawStringOnBuffer(
        buffer,
        line2,
        x2,
        17,
        scale
      );

    } else {

      // Long text = SAME continuous right-to-left scroll.
      // No splitting. No reversal.
      drawStringOnBuffer(
        buffer,
        line2,
        scrollX,
        17,
        scale
      );
    }
  }
}

// ============================================================
// SHIFT BYTE
// ============================================================

void shiftByte(uint8_t value) {

  for (int bit = 7; bit >= 0; bit--) {

    digitalWrite(
      DATA_PIN,
      (value >> bit) & 1
    );

    digitalWrite(
      CLK_PIN,
      HIGH
    );

    digitalWrite(
      CLK_PIN,
      LOW
    );
  }
}

// ============================================================
// REFRESH DISPLAY
// ============================================================

void refreshDisplayLoop() {

  // Fixed: Scan all 4 phases per refresh call to prevent flicker
  for (uint8_t phase = 0; phase < 4; phase++) {

    // Blank panel while shifting data.
    digitalWrite(OE_PIN, HIGH);

    // 1/4 scan address
    digitalWrite(
      A_PIN,
      phase & 0x01
    );

    digitalWrite(
      B_PIN,
      (phase >> 1) & 0x01
    );

    // ----------------------------------------------------------
    // SIX-PANEL CHAIN
    // Known-good row mapping
    // ----------------------------------------------------------

    for (int yBlock = 0; yBlock < 2; yBlock++) {

      int baseY =
        (1 - yBlock) * 16;

      for (int xByte = 0;
           xByte < STRIDE;
           xByte++) {

        int y0 =
          baseY + phase;

        int y1 =
          baseY + phase + 4;

        int y2 =
          baseY + phase + 8;

        int y3 =
          baseY + phase + 12;

        shiftByte(
          activeBuffer[
            y0 * STRIDE + xByte
          ]
        );

        shiftByte(
          activeBuffer[
            y1 * STRIDE + xByte
          ]
        );

        shiftByte(
          activeBuffer[
            y2 * STRIDE + xByte
          ]
        );

        shiftByte(
          activeBuffer[
            y3 * STRIDE + xByte
          ]
        );
      }
    }

    // ----------------------------------------------------------
    // LATCH
    // ----------------------------------------------------------

    digitalWrite(LAT_PIN, HIGH);

    delayMicroseconds(2);

    digitalWrite(LAT_PIN, LOW);

    // ----------------------------------------------------------
    // ENABLE DISPLAY
    // ----------------------------------------------------------

    digitalWrite(OE_PIN, LOW);

    // Fixed: Increased duty cycle time to stabilize brightness and stop stroboscopic flicker
    delayMicroseconds(300);
  }

  digitalWrite(OE_PIN, HIGH);
}

// ============================================================
// INITIALIZE DISPLAY
// ============================================================

void initDisplay() {

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(LAT_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  pinMode(A_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(LAT_PIN, LOW);

  // Start blank
  digitalWrite(OE_PIN, HIGH);

  digitalWrite(A_PIN, LOW);
  digitalWrite(B_PIN, LOW);

  clearBuffer(framebufferA);
  clearBuffer(framebufferB);

  // Initial message
  String message = "READY";

  int scale = 2;

  int textWidth = 
    getTextWidth(message, scale);

  int startX =
    (WIDTH - textWidth) / 2;

  // Vertically center the single line
  int startY =
    (HEIGHT - (7 * scale)) / 2;

  drawStringOnBuffer(
    framebufferA,
    message,
    startX,
    startY,
    scale
  );

  memcpy(
    framebufferB,
    framebufferA,
    BUFFER_SIZE
  );

  activeBuffer = framebufferA;
  drawBuffer   = framebufferB;

  Serial.println("P10 display initialized");
}

// ============================================================
// DISPLAY TASK
// ============================================================

void displayTask(void *pvParameters) {

  String currentLine1 = "READY";
  String currentLine2 = "";

  const int SCALE = 2;

  int scrollX = WIDTH;

  unsigned long lastScroll = millis();

  // Slightly slower movement for readability.
  const unsigned long SCROLL_INTERVAL = 80;

  bool frameNeedsBuild = true;

  while (true) {

    // --------------------------------------------------------
    // CHECK FOR NEW MESSAGE
    // --------------------------------------------------------

    if (newMessageFlag) {

      DisplayData local;

      if (xSemaphoreTake(
            displayMutex,
            pdMS_TO_TICKS(5)
          )) {

        local = sharedDisplay;

        newMessageFlag = false;

        xSemaphoreGive(displayMutex);

        currentLine1 = local.line1;
        currentLine2 = local.line2;

        // Every new scrolling message starts
        // completely from the right.
        scrollX = WIDTH;

        lastScroll = millis();

        frameNeedsBuild = true;
      }
    }

    // --------------------------------------------------------
    // TEXT WIDTHS
    // --------------------------------------------------------

    int width1 =
      getTextWidth(
        currentLine1,
        SCALE
      );

    int width2 =
      getTextWidth(
        currentLine2,
        SCALE
      );

    bool topNeedsScroll =
      (width1 > WIDTH);

    bool bottomNeedsScroll =
      (width2 > WIDTH);

    bool anyScrolling =
      topNeedsScroll ||
      bottomNeedsScroll;

    // --------------------------------------------------------
    // SCROLL
    // --------------------------------------------------------

    if (anyScrolling) {

      if (
        millis() - lastScroll >=
        SCROLL_INTERVAL
      ) {

        lastScroll = millis();

        scrollX--;

        // The same scroll position is used for BOTH lines.
        // This keeps both long lines moving in the same
        // right-to-left direction at exactly the same speed.

        int longestWidth =
          max(width1, width2);

        if (
          scrollX <
          -longestWidth
        ) {
          scrollX = WIDTH;
        }

        frameNeedsBuild = true;
      }
    }

    // --------------------------------------------------------
    // BUILD NEXT FRAME
    // --------------------------------------------------------

    if (frameNeedsBuild) {

      buildFrame(
        drawBuffer,
        currentLine1,
        currentLine2,
        scrollX,
        SCALE
      );

      // The display refresh and frame construction are
      // performed by this same task, so the swap occurs
      // between refresh operations and is safe here.

      uint8_t *temp =
        activeBuffer;

      activeBuffer =
        drawBuffer;

      drawBuffer =
        temp;

      frameNeedsBuild = false;
    }

    // --------------------------------------------------------
    // REFRESH
    // --------------------------------------------------------

    refreshDisplayLoop();
  }
}
