#include <Adafruit_NeoPixel.h>

#define BAUD_RATE 9600

// Dimmer
#define PIN_POT A0
#define DIMMER_PADDING 10 // updates state when changed by at least this value
uint16_t dimmerValue = 0;
bool dimmerValueChangedThisIteration = false;

// Light
#define PIN_LED 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN_LED, NEO_GRBW + NEO_KHZ800);

void setup() {
  pinMode(PIN_POT, INPUT);
  pinMode(PIN_LED, OUTPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(BAUD_RATE);
  Serial.println("setup() complete");
}

void loop() {
  // Input: Update state
  updateState();

  // Output
  printToSerial();
  output();

  // Iterations per ms
  delay(5);
}

void updateState() {
  static uint16_t lastDimmerValue = 0;
  static uint16_t thisDimmerValue;
  thisDimmerValue = analogRead(PIN_POT);

  if (thisDimmerValue == 0 || thisDimmerValue == 1023) {
    if (dimmerValue > 0 && dimmerValue < 1023) {
      lastDimmerValue = dimmerValue;
      dimmerValue = thisDimmerValue;
      dimmerValueChangedThisIteration = true;
    } else {
      dimmerValueChangedThisIteration = false;
    }
  } else if (DIMMER_PADDING <= abs(dimmerValue-thisDimmerValue)) {
    lastDimmerValue = dimmerValue;
    dimmerValue = thisDimmerValue;
    dimmerValueChangedThisIteration = true;
  } else {
    dimmerValueChangedThisIteration = false;
  }
}

void printToSerial() {
  if (dimmerValueChangedThisIteration) {
    Serial.println("Dimmer value: " + String(dimmerValue));
  }
}

void output() {
  static uint16_t fadeValue;
  static uint32_t color;
  if (dimmerValueChangedThisIteration) {
    if (dimmerValue <= 511) {
      fadeValue = map(dimmerValue, 0, 511, 0, 255);
      color = strip.Color(fadeValue, 0, 0);
    } else {
      fadeValue = map(dimmerValue-511, 0, 511, 0, 255);
      color = strip.Color(fadeValue, fadeValue, fadeValue);
    }
    for (int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
  }
}
