#include <Arduino.h>
#include "flite_arduino.h"
#include "driver/i2s.h"

// ============================================================
// ESP32 INTERNAL DAC
// GPIO25 = DAC1
// ============================================================

#define AUDIO_PIN 25

// ============================================================
// FLITE AUDIO
// ============================================================
//
// Flite voice produces 8 kHz PCM.
//
// The ESP32 internal I2S DAC will run at 16 kHz because
// 8 kHz is too low for the legacy DAC clock divider.
//
// Each Flite sample is therefore written twice.
// This preserves the original 8 kHz playback rate.
//
// ============================================================

#define FLITE_RATE 8000
#define I2S_RATE   16000

#define I2S_PORT I2S_NUM_0

// ============================================================
// AUDIO GAIN
// ============================================================

#define AUDIO_GAIN 3

// ============================================================
// I2S INITIALIZATION
// ============================================================

void initAudio() {

  Serial.println();
  Serial.println("========================================");
  Serial.println("INITIALIZING HARDWARE DAC");
  Serial.println("========================================");

  i2s_config_t config = {

    .mode =
      (i2s_mode_t)(
        I2S_MODE_MASTER |
        I2S_MODE_TX |
        I2S_MODE_DAC_BUILT_IN
      ),

    .sample_rate =
      I2S_RATE,

    .bits_per_sample =
      I2S_BITS_PER_SAMPLE_16BIT,

    .channel_format =
      I2S_CHANNEL_FMT_RIGHT_LEFT,

    .communication_format =
      I2S_COMM_FORMAT_STAND_I2S,

    .intr_alloc_flags =
      0,

    .dma_buf_count =
      8,

    .dma_buf_len =
      64,

    .use_apll =
      false,

    .tx_desc_auto_clear =
      true,

    .fixed_mclk =
      0
  };

  // ----------------------------------------------------------
  // Install I2S driver
  // ----------------------------------------------------------

  esp_err_t result =
    i2s_driver_install(
      I2S_PORT,
      &config,
      0,
      NULL
    );

  if (result != ESP_OK) {

    Serial.print(
      "I2S driver install failed: "
    );

    Serial.println(
      esp_err_to_name(result)
    );

    while (true) {
      delay(1000);
    }
  }

  // ----------------------------------------------------------
  // Use internal DAC
  // ----------------------------------------------------------

  result =
    i2s_set_pin(
      I2S_PORT,
      NULL
    );

  if (result != ESP_OK) {

    Serial.print(
      "I2S DAC pin setup failed: "
    );

    Serial.println(
      esp_err_to_name(result)
    );

    while (true) {
      delay(1000);
    }
  }

  // ----------------------------------------------------------
  // Enable DAC1
  //
  // DAC1 = GPIO25
  // ----------------------------------------------------------

  result =
    i2s_set_dac_mode(
      I2S_DAC_CHANNEL_LEFT_EN
    );

  if (result != ESP_OK) {

    Serial.print(
      "DAC mode failed: "
    );

    Serial.println(
      esp_err_to_name(result)
    );

    while (true) {
      delay(1000);
    }
  }

  // ----------------------------------------------------------
  // Clear DMA buffer
  // ----------------------------------------------------------

  i2s_zero_dma_buffer(
    I2S_PORT
  );

  Serial.println(
    "Hardware DAC ready."
  );

  Serial.println(
    "GPIO25 = DAC1"
  );

  Serial.println(
    "I2S hardware rate = 16000 Hz"
  );

  Serial.println(
    "Flite rate = 8000 Hz"
  );

  Serial.println(
    "Each Flite sample will be duplicated."
  );
}

// ============================================================
// FLITE AUDIO CALLBACK
// ============================================================

void audioCallback(
  size_t size,
  int16_t *values
) {

  // ----------------------------------------------------------
  // Maximum 256 Flite samples per block.
  //
  // Each sample becomes two I2S samples.
  //
  // Stereo I2S means another factor of 2.
  //
  // 256 × 2 × 2 = 1024 uint16_t values
  // ----------------------------------------------------------

  static uint16_t i2sBuffer[1024];

  size_t position = 0;

  while (position < size) {

    // --------------------------------------------------------
    // Process a maximum of 256 Flite samples at once
    // --------------------------------------------------------

    size_t block =
      size - position;

    if (block > 256)
      block = 256;

    size_t outputIndex = 0;

    // --------------------------------------------------------
    // Convert Flite PCM to DAC samples
    // --------------------------------------------------------

    for (size_t i = 0; i < block; i++) {

      int32_t sample =
        values[position + i];

      // ------------------------------------------------------
      // Software gain
      // ------------------------------------------------------

      sample =
        sample * AUDIO_GAIN;

      // ------------------------------------------------------
      // Prevent clipping
      // ------------------------------------------------------

      if (sample > 32767)
        sample = 32767;

      if (sample < -32768)
        sample = -32768;

      // ------------------------------------------------------
      // Signed PCM → unsigned DAC value
      // ------------------------------------------------------

      sample += 32768;

      uint16_t dacSample =
        (uint16_t)sample;

      // ------------------------------------------------------
      // Duplicate the sample.
      //
      // Flite = 8 kHz
      // I2S   = 16 kHz
      //
      // A A
      // B B
      // C C
      // ------------------------------------------------------

      i2sBuffer[outputIndex++] =
        dacSample;

      i2sBuffer[outputIndex++] =
        dacSample;

      // ------------------------------------------------------
      // Right channel
      //
      // Internal DAC1 uses the left DAC channel.
      // Keep right slot valid.
      // ------------------------------------------------------

      i2sBuffer[outputIndex++] =
        dacSample;

      i2sBuffer[outputIndex++] =
        dacSample;
    }

    // --------------------------------------------------------
    // Send samples to hardware DMA
    // --------------------------------------------------------

    size_t bytesWritten = 0;

    esp_err_t result =
      i2s_write(
        I2S_PORT,
        i2sBuffer,
        outputIndex * sizeof(uint16_t),
        &bytesWritten,
        portMAX_DELAY
      );

    if (result != ESP_OK) {

      Serial.print(
        "I2S write error: "
      );

      Serial.println(
        esp_err_to_name(result)
      );

      return;
    }

    position += block;
  }
}

// ============================================================
// FLITE OBJECT
// ============================================================

Flite *tts;

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println(
    "========================================"
  );

  Serial.println(
    "ESP32 OFFLINE ENGLISH TTS TEST"
  );

  Serial.println(
    "========================================"
  );

  // ----------------------------------------------------------
  // Start hardware audio
  // ----------------------------------------------------------

  initAudio();

  // ----------------------------------------------------------
  // Initialize Flite
  // ----------------------------------------------------------

  Serial.println();
  Serial.println(
    "Initializing Flite..."
  );

  tts =
    new Flite(
      audioCallback
    );

  Serial.println(
    "Flite initialized."
  );

  // ----------------------------------------------------------
  // Generate speech
  // ----------------------------------------------------------

  Serial.println();
  Serial.println(
    "Generating speech..."
  );

  tts->say(
    "Hello. This is an offline English text to speech test. "
    "The speech is being generated directly on the ESP32."
  );

  // ----------------------------------------------------------
  // Wait until DMA has transmitted everything
  // ----------------------------------------------------------

delay(1000);

  // ----------------------------------------------------------
  // Return DAC to silence
  // ----------------------------------------------------------

  i2s_zero_dma_buffer(
    I2S_PORT
  );

  Serial.println();
  Serial.println(
    "Speech generation finished."
  );

  Serial.println(
    "========================================"
  );
}

// ============================================================
// LOOP
// ============================================================

void loop() {

  delay(100);
}
