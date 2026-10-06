//======================================================================================
// ORION PROJECT - ESP32 + 6 x P10 HUB12
// 3 PANELS TOP + 3 PANELS BOTTOM
//
// TOP ROW    = GPIO26
// BOTTOM ROW = GPIO23
//
// Shared:
// CLK  = GPIO18
// LAT  = GPIO2
// OE   = GPIO22
// A    = GPIO19
// B    = GPIO21
//
// NO DMD32 LIBRARY
// NO EXTERNAL FONT FILES
//======================================================================================

#include <Arduino.h>

//======================================================================================
// PIN DEFINITIONS
//======================================================================================

#define PIN_DMD_nOE      22
#define PIN_DMD_A        19
#define PIN_DMD_B        21
#define PIN_DMD_CLK      18
#define PIN_DMD_SCLK     2

#define DATA_PIN_TOP     26
#define DATA_PIN_BOTTOM  23

//======================================================================================
// DISPLAY CONFIGURATION
//======================================================================================

#define PANELS_WIDE      3
#define PANELS_HIGH      1

#define PANEL_WIDTH      32
#define PANEL_HEIGHT     16

#define DISPLAY_WIDTH    (PANELS_WIDE * PANEL_WIDTH)   // 96
#define DISPLAY_HEIGHT   PANEL_HEIGHT                  // 16

#define GRAPHICS_NORMAL  0
#define GRAPHICS_INVERSE 1

// One 32x16 monochrome P10 panel = 64 bytes
#define PANEL_RAM_BYTES  64

//======================================================================================
// HUB12 CONTROL
//======================================================================================

#define LIGHT_DMD_ROW_01_05_09_13() \
  { \
    digitalWrite(PIN_DMD_B, LOW); \
    digitalWrite(PIN_DMD_A, LOW); \
  }

#define LIGHT_DMD_ROW_02_06_10_14() \
  { \
    digitalWrite(PIN_DMD_B, LOW); \
    digitalWrite(PIN_DMD_A, HIGH); \
  }

#define LIGHT_DMD_ROW_03_07_11_15() \
  { \
    digitalWrite(PIN_DMD_B, HIGH); \
    digitalWrite(PIN_DMD_A, LOW); \
  }

#define LIGHT_DMD_ROW_04_08_12_16() \
  { \
    digitalWrite(PIN_DMD_B, HIGH); \
    digitalWrite(PIN_DMD_A, HIGH); \
  }

#define LATCH_DMD_SHIFT_REG_TO_OUTPUT() \
  { \
    digitalWrite(PIN_DMD_SCLK, HIGH); \
    digitalWrite(PIN_DMD_SCLK, LOW); \
  }

#define OE_DMD_ROWS_OFF() \
  { \
    digitalWrite(PIN_DMD_nOE, HIGH); \
  }

#define OE_DMD_ROWS_ON() \
  { \
    digitalWrite(PIN_DMD_nOE, LOW); \
  }

//======================================================================================
// PIXEL LOOKUP
//======================================================================================

static const uint8_t pixelLookup[8] =
{
  0x80,
  0x40,
  0x20,
  0x10,
  0x08,
  0x04,
  0x02,
  0x01
};

//======================================================================================
// 5 x 7 FONT
//
// Characters supported:
// SPACE
// ! " # $ % & ' ( ) * + , - . /
// 0-9
// : ; < = > ? @
// A-Z
//
// Each character = 5 columns.
// Bit 0 = top pixel.
//======================================================================================

static const uint8_t font5x7[95][5] =
{
  {0x00,0x00,0x00,0x00,0x00}, // space
  {0x00,0x00,0x5F,0x00,0x00}, // !
  {0x00,0x07,0x00,0x07,0x00}, // "
  {0x14,0x7F,0x14,0x7F,0x14}, // #
  {0x24,0x2A,0x7F,0x2A,0x12}, // $
  {0x23,0x13,0x08,0x64,0x62}, // %
  {0x36,0x49,0x55,0x22,0x50}, // &
  {0x00,0x05,0x03,0x00,0x00}, // '
  {0x00,0x1C,0x22,0x41,0x00}, // (
  {0x00,0x41,0x22,0x1C,0x00}, // )
  {0x14,0x08,0x3E,0x08,0x14}, // *
  {0x08,0x08,0x3E,0x08,0x08}, // +
  {0x00,0x50,0x30,0x00,0x00}, // ,
  {0x08,0x08,0x08,0x08,0x08}, // -
  {0x00,0x60,0x60,0x00,0x00}, // .
  {0x20,0x10,0x08,0x04,0x02}, // /

  {0x3E,0x51,0x49,0x45,0x3E}, // 0
  {0x00,0x42,0x7F,0x40,0x00}, // 1
  {0x42,0x61,0x51,0x49,0x46}, // 2
  {0x21,0x41,0x45,0x4B,0x31}, // 3
  {0x18,0x14,0x12,0x7F,0x10}, // 4
  {0x27,0x45,0x45,0x45,0x39}, // 5
  {0x3C,0x4A,0x49,0x49,0x30}, // 6
  {0x01,0x71,0x09,0x05,0x03}, // 7
  {0x36,0x49,0x49,0x49,0x36}, // 8
  {0x06,0x49,0x49,0x29,0x1E}, // 9

  {0x00,0x36,0x36,0x00,0x00}, // :
  {0x00,0x56,0x36,0x00,0x00}, // ;
  {0x08,0x14,0x22,0x41,0x00}, // <
  {0x14,0x14,0x14,0x14,0x14}, // =
  {0x00,0x41,0x22,0x14,0x08}, // >
  {0x02,0x01,0x51,0x09,0x06}, // ?
  {0x32,0x49,0x79,0x41,0x3E}, // @

  {0x7E,0x11,0x11,0x11,0x7E}, // A
  {0x7F,0x49,0x49,0x49,0x36}, // B
  {0x3E,0x41,0x41,0x41,0x22}, // C
  {0x7F,0x41,0x41,0x22,0x1C}, // D
  {0x7F,0x49,0x49,0x49,0x41}, // E
  {0x7F,0x09,0x09,0x09,0x01}, // F
  {0x3E,0x41,0x49,0x49,0x7A}, // G
  {0x7F,0x08,0x08,0x08,0x7F}, // H
  {0x00,0x41,0x7F,0x41,0x00}, // I
  {0x20,0x40,0x41,0x3F,0x01}, // J
  {0x7F,0x08,0x14,0x22,0x41}, // K
  {0x7F,0x40,0x40,0x40,0x40}, // L
  {0x7F,0x02,0x0C,0x02,0x7F}, // M
  {0x7F,0x04,0x08,0x10,0x7F}, // N
  {0x3E,0x41,0x41,0x41,0x3E}, // O
  {0x7F,0x09,0x09,0x09,0x06}, // P
  {0x3E,0x41,0x51,0x21,0x5E}, // Q
  {0x7F,0x09,0x19,0x29,0x46}, // R
  {0x46,0x49,0x49,0x49,0x31}, // S
  {0x01,0x01,0x7F,0x01,0x01}, // T
  {0x3F,0x40,0x40,0x40,0x3F}, // U
  {0x1F,0x20,0x40,0x20,0x1F}, // V
  {0x3F,0x40,0x38,0x40,0x3F}, // W
  {0x63,0x14,0x08,0x14,0x63}, // X
  {0x07,0x08,0x70,0x08,0x07}, // Y
  {0x61,0x51,0x49,0x45,0x43}, // Z

  {0x3E,0x45,0x49,0x51,0x3E}, // [
  {0x00,0x00,0x7F,0x00,0x00}, // backslash substitute
  {0x3E,0x51,0x49,0x45,0x3E}, // ]
  {0x08,0x04,0x02,0x04,0x08}, // ^
  {0x40,0x40,0x40,0x40,0x40}, // _
  {0x00,0x03,0x07,0x00,0x00}, // `

  {0x20,0x54,0x54,0x54,0x78}, // a
  {0x7F,0x48,0x44,0x44,0x38}, // b
  {0x38,0x44,0x44,0x44,0x20}, // c
  {0x38,0x44,0x44,0x48,0x7F}, // d
  {0x38,0x54,0x54,0x54,0x18}, // e
  {0x08,0x7E,0x09,0x01,0x02}, // f
  {0x0C,0x52,0x52,0x52,0x3E}, // g
  {0x7F,0x08,0x04,0x04,0x78}, // h
  {0x00,0x44,0x7D,0x40,0x00}, // i
  {0x20,0x40,0x44,0x3D,0x00}, // j
  {0x7F,0x10,0x28,0x44,0x00}, // k
  {0x00,0x41,0x7F,0x40,0x00}, // l
  {0x7C,0x04,0x18,0x04,0x78}, // m
  {0x7C,0x08,0x04,0x04,0x78}, // n
  {0x38,0x44,0x44,0x44,0x38}, // o
  {0x7C,0x14,0x14,0x14,0x08}, // p
  {0x08,0x14,0x14,0x18,0x7C}, // q
  {0x7C,0x08,0x04,0x04,0x08}, // r
  {0x48,0x54,0x54,0x54,0x20}, // s
  {0x04,0x3F,0x44,0x40,0x20}, // t
  {0x3C,0x40,0x40,0x20,0x7C}, // u
  {0x1C,0x20,0x40,0x20,0x1C}, // v
  {0x3C,0x40,0x30,0x40,0x3C}, // w
  {0x44,0x28,0x10,0x28,0x44}, // x
  {0x0C,0x50,0x50,0x50,0x3C}, // y
  {0x44,0x64,0x54,0x4C,0x44}, // z

  {0x08,0x36,0x41,0x00,0x00}, // {
  {0x00,0x00,0x7F,0x00,0x00}, // |
  {0x00,0x41,0x36,0x08,0x00}, // }
  {0x08,0x04,0x08,0x10,0x08}  // ~
};

//======================================================================================
// DMD CLASS
//======================================================================================

class DMD
{
public:

  DMD(byte panelsWide, byte panelsHigh)
  {
    DisplaysWide  = panelsWide;
    DisplaysHigh  = panelsHigh;
    DisplaysTotal = DisplaysWide * DisplaysHigh;

    // 4 scan rows x bytes per panel row.
    rowBytes = DisplaysTotal * 4;

    ramSize = DisplaysTotal * PANEL_RAM_BYTES;

    bDMDScreenRAM = (uint8_t*)malloc(ramSize);

    if (bDMDScreenRAM != nullptr)
    {
      clearScreen(true);
    }

    bDMDByte = 0;
  }

  ~DMD()
  {
    if (bDMDScreenRAM != nullptr)
    {
      free(bDMDScreenRAM);
      bDMDScreenRAM = nullptr;
    }
  }

  //====================================================================================
  // CLEAR
  //====================================================================================

  void clearScreen(bool normal)
  {
    if (bDMDScreenRAM == nullptr)
      return;

    memset(
      bDMDScreenRAM,
      normal ? 0xFF : 0x00,
      ramSize
    );
  }

  //====================================================================================
  // WRITE PIXEL
  //====================================================================================

  void writePixel(
    unsigned int x,
    unsigned int y,
    byte graphicsMode,
    byte pixel
  )
  {
    if (bDMDScreenRAM == nullptr)
      return;

    unsigned int width =
      DISPLAY_WIDTH;

    unsigned int height =
      DISPLAY_HEIGHT;

    if (x >= width || y >= height)
      return;

    // Each 16-row P10 panel is stored as:
    //
    // byte row 0-3   = physical rows 0,4,8,12
    // byte row 4-7   = physical rows 1,5,9,13
    // byte row 8-11  = physical rows 2,6,10,14
    // byte row 12-15 = physical rows 3,7,11,15
    //
    // The original working scanner uses this same arrangement.

    uint8_t scanRow = y & 0x03;

    uint8_t group =
      y >> 2;

    uint8_t panel =
      x >> 5;

    uint8_t localX =
      x & 0x1F;

    uint8_t byteInPanel =
      localX >> 3;

    uint8_t bit =
      localX & 0x07;

    // Panel storage is 64 bytes:
    //
    // group 0: 0..3
    // group 1: 4..7
    // group 2: 8..11
    // group 3: 12..15
    //
    // Each group contains 4 bytes because each panel is 32 pixels wide.

    uint16_t index =
      panel * PANEL_RAM_BYTES +
      group * 4 +
      byteInPanel;

    uint8_t mask =
      pixelLookup[bit];

    if (graphicsMode == GRAPHICS_NORMAL)
    {
      if (pixel)
        bDMDScreenRAM[index] &= ~mask;
      else
        bDMDScreenRAM[index] |= mask;
    }
    else
    {
      if (!pixel)
        bDMDScreenRAM[index] &= ~mask;
      else
        bDMDScreenRAM[index] |= mask;
    }
  }

  //====================================================================================
  // DRAW CHARACTER
  //====================================================================================

  int drawChar(
    int x,
    int y,
    char character,
    byte graphicsMode
  )
  {
    if (character < 32 || character > 126)
      character = '?';

    const uint8_t* glyph =
      font5x7[character - 32];

    for (uint8_t column = 0; column < 5; column++)
    {
      uint8_t data =
        glyph[column];

      for (uint8_t row = 0; row < 7; row++)
      {
        bool pixel =
          data & (1 << row);

        if (x + column >= 0 &&
            y + row >= 0)
        {
          writePixel(
            x + column,
            y + row,
            graphicsMode,
            pixel
          );
        }
      }
    }

    return 5;
  }

  //====================================================================================
  // CHARACTER WIDTH
  //====================================================================================

  int charWidth(char character)
  {
    return 5;
  }

  //====================================================================================
  // DRAW STRING
  //====================================================================================

  void drawString(
    int x,
    int y,
    const char* text,
    byte length,
    byte graphicsMode
  )
  {
    if (text == nullptr)
      return;

    int currentX = x;

    for (byte i = 0; i < length; i++)
    {
      char c = text[i];

      drawChar(
        currentX,
        y,
        c,
        graphicsMode
      );

      currentX += 6;
    }
  }

  //====================================================================================
  // TEXT WIDTH
  //====================================================================================

  int getTextWidth(const char* text)
  {
    if (text == nullptr)
      return 0;

    int width = strlen(text);

    if (width == 0)
      return 0;

    // 5 pixels character + 1 pixel spacing.
    return width * 6;
  }

  //====================================================================================
  // DUAL DATA SCAN
  //
  // This is the important part.
  //
  // Two completely independent 96x16 framebuffers are shifted simultaneously:
  //
  // dataPinThis  -> one physical row
  // dataPinOther -> other physical row
  //
  // Both use the same CLK/LAT/OE/A/B.
  //====================================================================================

  void scanDualBitBang(
    DMD &otherChain,
    byte dataPinThis,
    byte dataPinOther
  )
  {
    if (bDMDScreenRAM == nullptr ||
        otherChain.bDMDScreenRAM == nullptr)
      return;

    // Turn display OFF while shifting/latching.
    OE_DMD_ROWS_OFF();

    // Four scan groups.
    //
    // Each group contains:
    //
    //   3 panels x 4 bytes = 12 bytes
    //
    // For every byte position we send:
    //
    // row group 3
    // row group 2
    // row group 1
    // row group 0
    //
    // This follows the original working scanner structure.

    int offsetThis =
      bDMDByte * rowBytes;

    int offsetOther =
      otherChain.bDMDByte * otherChain.rowBytes;

    for (int i = 0; i < rowBytes; i++)
    {
      uint8_t byteThis =
        bDMDScreenRAM[
          offsetThis + i
        ];

      uint8_t byteOther =
        otherChain.bDMDScreenRAM[
          offsetOther + i
        ];

      for (int bit = 7; bit >= 0; bit--)
      {
        digitalWrite(
          dataPinThis,
          (byteThis >> bit) & 0x01
        );

        digitalWrite(
          dataPinOther,
          (byteOther >> bit) & 0x01
        );

        digitalWrite(
          PIN_DMD_CLK,
          HIGH
        );

        digitalWrite(
          PIN_DMD_CLK,
          LOW
        );
      }
    }

    // Latch shifted data.
    LATCH_DMD_SHIFT_REG_TO_OUTPUT();

    // Select next physical scan row.
    switch (bDMDByte)
    {
      case 0:
        LIGHT_DMD_ROW_01_05_09_13();
        bDMDByte = 1;
        break;

      case 1:
        LIGHT_DMD_ROW_02_06_10_14();
        bDMDByte = 2;
        break;

      case 2:
        LIGHT_DMD_ROW_03_07_11_15();
        bDMDByte = 3;
        break;

      case 3:
        LIGHT_DMD_ROW_04_08_12_16();
        bDMDByte = 0;
        break;
    }

    // Keep both DMD objects synchronized.
    otherChain.bDMDByte =
      bDMDByte;

    // Turn display ON.
    OE_DMD_ROWS_ON();
  }

private:

  uint8_t* bDMDScreenRAM = nullptr;

  uint8_t DisplaysWide  = 0;
  uint8_t DisplaysHigh  = 0;
  uint8_t DisplaysTotal = 0;

  uint16_t rowBytes = 0;
  uint16_t ramSize  = 0;

  volatile uint8_t bDMDByte = 0;
};

//======================================================================================
// DISPLAY OBJECTS
//======================================================================================

DMD dmd_top(
  PANELS_WIDE,
  PANELS_HIGH
);

DMD dmd_bottom(
  PANELS_WIDE,
  PANELS_HIGH
);

//======================================================================================
// TEXT
//======================================================================================

const char* TOP_TEXT =
  "GOD I THANK YOU";

const char* BOTTOM_TEXT =
  "Jesus is the greatest, nobody like him";

//======================================================================================
// SCROLL SETTINGS
//======================================================================================

int topScrollX =
  DISPLAY_WIDTH;

int bottomScrollX =
  DISPLAY_WIDTH;

unsigned long lastScrollTime =
  0;

const unsigned long SCROLL_INTERVAL =
  50;

//======================================================================================
// GET TEXT WIDTH
//======================================================================================

int getTextWidth(
  DMD &display,
  const char* text
)
{
  return display.getTextWidth(text);
}

//======================================================================================
// SETUP
//======================================================================================

void setup()
{
  Serial.begin(115200);

  //------------------------------------------------------------------------
  // HUB12 CONTROL PINS
  //------------------------------------------------------------------------

  pinMode(PIN_DMD_A, OUTPUT);
  pinMode(PIN_DMD_B, OUTPUT);

  pinMode(PIN_DMD_CLK, OUTPUT);
  pinMode(PIN_DMD_SCLK, OUTPUT);

  pinMode(PIN_DMD_nOE, OUTPUT);

  //------------------------------------------------------------------------
  // DATA PINS
  //------------------------------------------------------------------------

  pinMode(DATA_PIN_TOP, OUTPUT);
  pinMode(DATA_PIN_BOTTOM, OUTPUT);

  //------------------------------------------------------------------------
  // INITIAL STATES
  //------------------------------------------------------------------------

  digitalWrite(
    PIN_DMD_A,
    LOW
  );

  digitalWrite(
    PIN_DMD_B,
    LOW
  );

  digitalWrite(
    PIN_DMD_CLK,
    LOW
  );

  digitalWrite(
    PIN_DMD_SCLK,
    LOW
  );

  digitalWrite(
    DATA_PIN_TOP,
    HIGH
  );

  digitalWrite(
    DATA_PIN_BOTTOM,
    HIGH
  );

  // OE is active LOW on this panel.
  // Start with display OFF.
  digitalWrite(
    PIN_DMD_nOE,
    HIGH
  );

  //------------------------------------------------------------------------
  // CLEAR BOTH ROWS
  //------------------------------------------------------------------------

  dmd_top.clearScreen(true);
  dmd_bottom.clearScreen(true);

  topScrollX =
    DISPLAY_WIDTH;

  bottomScrollX =
    DISPLAY_WIDTH;

  lastScrollTime =
    millis();

  Serial.println();
  Serial.println("==============================");
  Serial.println("ORION P10 DISPLAY STARTED");
  Serial.println("==============================");
  Serial.println("TOP DATA    : GPIO26");
  Serial.println("BOTTOM DATA : GPIO23");
  Serial.println("CLK         : GPIO18");
  Serial.println("LAT         : GPIO2");
  Serial.println("OE          : GPIO22");
  Serial.println("A           : GPIO19");
  Serial.println("B           : GPIO21");
  Serial.println("==============================");
}

//======================================================================================
// LOOP
//======================================================================================

void loop()
{
  //------------------------------------------------------------------------
  // CALCULATE TEXT WIDTHS
  //------------------------------------------------------------------------

  int topWidth =
    getTextWidth(
      dmd_top,
      TOP_TEXT
    );

  int bottomWidth =
    getTextWidth(
      dmd_bottom,
      BOTTOM_TEXT
    );

  //------------------------------------------------------------------------
  // UPDATE SCROLL POSITIONS
  //------------------------------------------------------------------------

  if (
    millis() - lastScrollTime >=
    SCROLL_INTERVAL
  )
  {
    lastScrollTime =
      millis();

    // TOP
    if (topWidth > DISPLAY_WIDTH)
    {
      topScrollX--;

      if (topScrollX < -topWidth)
      {
        topScrollX =
          DISPLAY_WIDTH;
      }
    }

    // BOTTOM
    if (bottomWidth > DISPLAY_WIDTH)
    {
      bottomScrollX--;

      if (bottomScrollX < -bottomWidth)
      {
        bottomScrollX =
          DISPLAY_WIDTH;
      }
    }
  }

  //------------------------------------------------------------------------
  // CLEAR BOTH FRAMEBUFFERS
  //------------------------------------------------------------------------

  dmd_top.clearScreen(true);
  dmd_bottom.clearScreen(true);

  //------------------------------------------------------------------------
  // TOP ROW
  //------------------------------------------------------------------------

  int topLength =
    strlen(TOP_TEXT);

  if (topWidth <= DISPLAY_WIDTH)
  {
    int topX =
      (DISPLAY_WIDTH - topWidth) / 2;

    dmd_top.drawString(
      topX,
      0,
      TOP_TEXT,
      topLength,
      GRAPHICS_NORMAL
    );
  }
  else
  {
    dmd_top.drawString(
      topScrollX,
      0,
      TOP_TEXT,
      topLength,
      GRAPHICS_NORMAL
    );
  }

  //------------------------------------------------------------------------
  // BOTTOM ROW
  //------------------------------------------------------------------------

  int bottomLength =
    strlen(BOTTOM_TEXT);

  if (bottomWidth <= DISPLAY_WIDTH)
  {
    int bottomX =
      (DISPLAY_WIDTH - bottomWidth) / 2;

    dmd_bottom.drawString(
      bottomX,
      0,
      BOTTOM_TEXT,
      bottomLength,
      GRAPHICS_NORMAL
    );
  }
  else
  {
    dmd_bottom.drawString(
      bottomScrollX,
      0,
      BOTTOM_TEXT,
      bottomLength,
      GRAPHICS_NORMAL
    );
  }

  //------------------------------------------------------------------------
  // REFRESH BOTH PHYSICAL ROWS
  //------------------------------------------------------------------------

  for (int pass = 0; pass < 4; pass++)
  {
    dmd_top.scanDualBitBang(
      dmd_bottom,
      DATA_PIN_TOP,
      DATA_PIN_BOTTOM
    );
  }
}
