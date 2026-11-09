#include <Arduino.h>

#include "AudioTools.h"

#include "playPling.h"

#define SAMPLE_RATE 44100

static I2SStream i2s;

bool audioSetup() {
  auto config = i2s.defaultConfig(TX_MODE);
  config.pin_bck  = MAX_BCLK;
  config.pin_ws   = MAX_LRC;
  config.pin_data = MAX_DIN;
  config.sample_rate = SAMPLE_RATE;
  config.channels = 2;
  config.bits_per_sample = 16;
  if (!i2s.begin(config)) {
    Serial.println("Failed to initialize I2S audio");
    return false;
  }
  return true;
}

void playPling(float frequency, float duration_sec) {
  int total_samples = SAMPLE_RATE * duration_sec;

  for (int i = 0; i < total_samples; i++) {
    float t = (float)i / SAMPLE_RATE;
    // Exponential decay creates a bell/pling envelope
    float envelope = exp(-6.0 * t);
    int16_t sample = (int16_t)(sin(2.0 * PI * frequency * t) * 30000 * envelope);

    // Stereo frame (Left and Right channels)
    int16_t stereo_sample[2] = {sample, sample};
    if (i2s.write(reinterpret_cast<const uint8_t *>(stereo_sample), sizeof(stereo_sample)) != sizeof(stereo_sample)) {
      Serial.println("Failed to write I2S audio");
      return;
    }
  }

  // Flush buffer with silence to prevent a trailing click
  int16_t silence[2] = {0, 0};
  for (int i = 0; i < 256; i++) {
    if (i2s.write(reinterpret_cast<const uint8_t *>(silence), sizeof(silence)) != sizeof(silence)) {
      Serial.println("Failed to write I2S silence");
      return;
    }
  }
}