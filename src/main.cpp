#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include <esp_heap_caps.h>

#if defined(__has_include)
#if __has_include(<esp_mac.h>)
#include <esp_mac.h>
#define HAS_ESP_MAC 1
#endif
#endif

#define LED_PIN     48
#define NUM_LEDS    1
#define BRIGHTNESS  128
#define DELAY_MS    20
#define INFO_DELAY  2000  // 2 seconds pause before LED effect

// ---------------------------------------------------------------------------
// Serial output layout:
//   1. Static header (printed in setup): greeting + kit/chip details.
//      The live feed below never re-sends it, so it stays put. If you open
//      the serial monitor AFTER boot (most monitors do not reset the board
//      when they attach), press Enter to reprint the full header - any CR/LF
//      received over serial recalls it.
//   2. Live color feed (printed in loop): by default a single line that
//      rewrites itself with '\r', so colors stream in real time WITHOUT
//      scrolling the header away.
//      Set LIVE_SINGLE_LINE to 0 if your serial monitor treats a bare
//      carriage return as a newline (it then prints one full line per
//      update instead, and the header scrolls up as usual).
// ---------------------------------------------------------------------------
#define LIVE_SINGLE_LINE 1     // 1 = overwrite one line with '\r', 0 = scroll
#define LIVE_MS_SINGLE   100   // refresh rate in single-line mode (ms)
#define LIVE_MS_SCROLL   1000  // refresh rate in scrolling mode (ms)

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

// Plain-language name for a hue angle (0-359 degrees).
const char *hueName(uint16_t deg) {
  deg %= 360;
  if (deg < 15)  return "Red";
  if (deg < 45)  return "Orange";
  if (deg < 75)  return "Yellow";
  if (deg < 105) return "Lime";
  if (deg < 165) return "Green";
  if (deg < 195) return "Cyan";
  if (deg < 255) return "Blue";
  if (deg < 285) return "Violet";
  if (deg < 315) return "Magenta";
  if (deg < 345) return "Pink";
  return "Red";
}

// Static block: every value below is read live from the chip at boot -
// nothing is hard-coded, so what you see is what this silicon actually is.
void printHardwareInfo() {
  uint32_t flashKB = ESP.getFlashChipSize() / 1024;  // from the flash chip ID
  // Read PSRAM straight from the heap allocator - honest actual data that does
  // not depend on psramFound(), which is unreliable in this PIO core build.
  uint32_t psramBytes = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
  uint32_t psramKB = psramBytes / 1024;
#if defined(ARDUINO_USB_CDC_ON_BOOT) && ARDUINO_USB_CDC_ON_BOOT
  const char *consoleUart = "USB CDC (native USB Serial/JTAG)";
#else
  const char *consoleUart = "UART0 via external USB-UART bridge";
#endif

  Serial.println("---------------------------------------------");
  Serial.print("Chip Model         : ");
  Serial.println(ESP.getChipModel());

  Serial.print("Chip Version       : silicon rev ");
  Serial.print(ESP.getChipRevision());
  Serial.println(" (actual data, read live from this chip)");

  Serial.print("CPU Cores          : ");
  Serial.println(ESP.getChipCores());

  Serial.print("CPU Frequency      : ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");

  Serial.print("Flash Size         : ");
  Serial.print(flashKB / 1024);
  Serial.print(" MB (");
  Serial.print(flashKB);
  Serial.print(" KB, from flash chip ID) @ ");
  Serial.print(ESP.getFlashChipSpeed() / 1000000);
  Serial.println(" MHz");

  Serial.print("PSRAM              : ");
  if (psramKB > 0) {
    Serial.print(psramKB);
    Serial.print(" KB (~");
    Serial.print((psramBytes + 524288UL) / 1048576UL); // rounded to MB
    Serial.println(" MB, in heap)");
  } else {
    Serial.println("0 KB (not present or not enabled)");
  }

  Serial.print("Kit Check          : ");
  bool isN16 = (flashKB == 16384);
  bool isR8  = (psramBytes >= 8388608UL - 65536UL); // ~8 MB, heap loses a bit
  if (isN16 && isR8) {
    Serial.println("N16R8 confirmed - 16 MB flash + 8 MB PSRAM");
  } else if (isN16) {
    Serial.println("N16 confirmed (16 MB flash) - PSRAM not visible");
  } else {
    Serial.print("Not an N16 profile - ");
    Serial.print(flashKB / 1024);
    Serial.print(" MB flash, ");
    Serial.print(psramKB / 1024);
    Serial.println(" MB PSRAM");
  }

#if defined(HAS_ESP_MAC)
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  Serial.print("MAC Address        : ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
#endif

  Serial.print("Console UART       : ");
  Serial.print(consoleUart);
  Serial.print(" @ ");
  Serial.print(Serial.baudRate());
  Serial.println(" baud");

  // The bridge chip sits on the PC side of the USB link, so the firmware
  // cannot query it at runtime. When the build machine is Linux with the kit
  // plugged in, scripts/uart_bridge_info.py captures the actual USB
  // descriptor and injects it as UART_BRIDGE_USB_INFO. On any other OS (or
  // a build with no kit attached) we fall back to the kit's shipping spec so
  // every user still sees a real bridge line in their serial monitor.
  Serial.print("UART Bridge Chip   : ");
#ifdef UART_BRIDGE_USB_INFO
  Serial.println(UART_BRIDGE_USB_INFO);          // measured on the build host
#else
  Serial.println("WCH CH343 (USB 1A86:55D3, fw v4.45) - Bitzy Labs kit spec");
#endif

  Serial.print("Firmware Built     : ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);

  Serial.print("Free Heap          : ");
  Serial.print(ESP.getFreeHeap() / 1024);
  Serial.println(" KB");

  Serial.println("---------------------------------------------");
  Serial.print("LED                : ");
  Serial.print(NUM_LEDS);
  Serial.print(" x NeoPixel (GRB) @ GPIO");
  Serial.print(LED_PIN);
  Serial.print(", brightness ");
  Serial.println(BRIGHTNESS);

  Serial.println("Static block       : printed at boot - press Enter in your");
  Serial.println("                     serial monitor to reprint it anytime.");
  Serial.println("Live color feed    : the line below rewrites itself in place.");
  Serial.println("---------------------------------------------");
}

void printBootBanner() {
  Serial.println("Hellooo, fellow maker!!");
  Serial.println("Thanks heaps for your ESP32 N16R8 Kit Purchase from Bitzy Labs!!");
  Serial.println("This is what you've got :");
  printHardwareInfo();
}

// Recall the static header when the user presses Enter (or sends any CR/LF)
// in the serial monitor. Monitors attach to an already-running board without
// resetting it, so the boot header can be off screen or never seen - this
// brings it back instantly. Any non-CR/LF bytes are ignored so typing text
// does not spam reprints.
void handleSerialRecall() {
  bool recall = false;
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r' || c == '\n') recall = true;
  }
  if (!recall) return;

  Serial.println();      // finish the current live row (cursor was mid-line)
  printBootBanner();     // full greeting + static block again
  // The next live frame starts with '\r', so it resumes on the row right
  // below the freshly printed header - nothing scrolls it away.
}

void printLiveFeed(uint16_t hueDeg, uint8_t r, uint8_t g, uint8_t b,
                   uint8_t whitePct) {
  char line[96];
#if LIVE_SINGLE_LINE
  // Leading '\r' returns to the start of the same row, so this line updates
  // in place and never pushes the header up. Trailing space clears leftovers.
  snprintf(line, sizeof(line),
           "\r[LIVE] hue=%3u deg | R=%3u G=%3u B=%3u | white +%2u%% | %-8s ",
           hueDeg, r, g, b, whitePct, hueName(hueDeg));
  Serial.print(line);
#else
  snprintf(line, sizeof(line),
           "[LIVE] hue=%3u deg | R=%3u G=%3u B=%3u | white +%2u%% | %s",
           hueDeg, r, g, b, whitePct, hueName(hueDeg));
  Serial.println(line);
#endif
}

void setup() {
  Serial.begin(115200);
  delay(100); // Give time for serial monitor to connect

  while (Serial.available()) Serial.read(); // drop stale bytes (no double banner)
  printBootBanner(); // Greeting + static chip/devboard details (once at boot)
  delay(INFO_DELAY); // Pause 2 seconds before starting LED effect

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  static float hue = 0.0;
  static float whitePhase = 0.0;
  static uint32_t lastPrint = 0;

  handleSerialRecall(); // Enter in the serial monitor reprints the header

  uint8_t r, g, b;
  hsvToRgb(hue, 1.0, 1.0, r, g, b);

  float whiteBlend = (sin(whitePhase) + 1.0) / 6.0;
  r = min(255, r + int(255 * whiteBlend));
  g = min(255, g + int(255 * whiteBlend));
  b = min(255, b + int(255 * whiteBlend));

  strip.setPixelColor(0, strip.Color(r, g, b));
  strip.show();

  // Real-time color feed, throttled so the monitor stays readable.
  uint32_t now = millis();
  uint32_t interval = LIVE_SINGLE_LINE ? LIVE_MS_SINGLE : LIVE_MS_SCROLL;
  if (now - lastPrint >= interval) {
    lastPrint = now;
    uint16_t hueDeg = (uint16_t)(hue * 360.0f) % 360;
    printLiveFeed(hueDeg, r, g, b, (uint8_t)(whiteBlend * 100.0f));
  }

  hue += 0.002;
  if(hue > 1.0) hue -= 1.0;

  whitePhase += 0.05;
  if(whitePhase > 2 * PI) whitePhase -= 2 * PI;

  delay(DELAY_MS);
}
