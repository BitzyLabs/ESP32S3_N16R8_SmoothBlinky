#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#define LED_PIN     48
#define NUM_LEDS    1
#define BRIGHTNESS 128
#define DELAY_MS    20
#define INFO_DELAY 2000  // 2 seconds pause before LED effect

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint8_t gamma8(uint8_t x) {
  return pow(x / 255.0, 2.8) * 255.0 + 0.5;
}

void hsvToRgb(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
  int i = int(h * 6);
  float f = h * 6 - i;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);

  switch(i % 6) {
    case 0: r = v*255; g = t*255; b = p*255; break;
    case 1: r = q*255; g = v*255; b = p*255; break;
    case 2: r = p*255; g = v*255; b = t*255; break;
    case 3: r = p*255; g = q*255; b = v*255; break;
    case 4: r = t*255; g = p*255; b = v*255; break;
    case 5: r = v*255; g = p*255; b = q*255; break;
  }

  r = gamma8(r);
  g = gamma8(g);
  b = gamma8(b);
}

void printHardwareInfo() {
  Serial.println("=== ESP32-S3 Hardware Info ===");
  Serial.print("Chip Model: "); Serial.println(ESP.getChipModel());
  Serial.print("CPU Cores: "); Serial.println(ESP.getChipCores());
  Serial.print("CPU Frequency: "); Serial.print(ESP.getCpuFreqMHz()); Serial.println(" MHz");
  Serial.print("Flash Size: "); Serial.print(ESP.getFlashChipSize() / 1024); Serial.println(" KB");
  Serial.print("Free Heap: "); Serial.print(ESP.getFreeHeap() / 1024); Serial.println(" KB");
  Serial.println("==============================");
}

void setup() {
  Serial.begin(115200);
  delay(100); // Give time for serial monitor to connect

  printHardwareInfo(); // Print hardware info
  delay(INFO_DELAY);   // Pause 2 seconds before starting LED effect

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  static float hue = 0.0;
  static float whitePhase = 0.0;

  uint8_t r, g, b;
  hsvToRgb(hue, 1.0, 1.0, r, g, b);

  float whiteBlend = (sin(whitePhase) + 1.0) / 6.0;
  r = min(255, r + int(255 * whiteBlend));
  g = min(255, g + int(255 * whiteBlend));
  b = min(255, b + int(255 * whiteBlend));

  strip.setPixelColor(0, strip.Color(r, g, b));
  strip.show();

  hue += 0.002;
  if(hue > 1.0) hue -= 1.0;

  whitePhase += 0.05;
  if(whitePhase > 2 * PI) whitePhase -= 2 * PI;

  delay(DELAY_MS);
}
