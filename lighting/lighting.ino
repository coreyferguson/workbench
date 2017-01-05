#include <Adafruit_NeoPixel.h>

#define BAUD_RATE 9600

// Motion Sensor
// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
#define PIN_PIR A2

// Light
#define PIN_LED 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN_LED, NEO_GRBW + NEO_KHZ800);
uint32_t white = strip.Color(255, 255, 255);
uint32_t black = strip.Color(0, 0, 0);

void setup() {
  pinMode(PIN_PIR, INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(BAUD_RATE);
  Serial.println("setup() complete");
}

void loop() {
  static bool pirActive;
  static bool pirWasActive;
  pirWasActive = pirActive;
  pirActive = digitalRead(PIN_PIR);

  Serial.println(pirActive, DEC);

  if (!pirWasActive && pirActive) {
    for (int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, white);
    }
    strip.show();
  } else if (pirWasActive && !pirActive) {
    for (int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, black);
    }
    strip.show();
  }

  delay(50);
}
