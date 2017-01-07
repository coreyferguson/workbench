#include <Adafruit_NeoPixel.h>

#define BAUD_RATE 9600

// Dimmer
#define PIN_POT A0
#define DIMMER_PADDING 10 // updates state when changed by at least this value
uint16_t dimmerValue = 0;
bool dimmerValueChangedThisIteration = false;
bool animationMode = false;

// Light
#define PIN_LED 6
#define NUM_LEDS_ON_IN_ANIMATION 15
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN_LED, NEO_GRBW + NEO_KHZ800);
static uint32_t color = strip.Color(255,0,0);

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

  // animation mode
  if (thisDimmerValue == 1023) {
    animationMode = true;
  } else {
    animationMode = false;
  }

  // dimmer value queue
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
  if (animationMode) {
    outputAnimation();
  } else {
    outputLight();
  }
}

void outputLight() {
  static uint16_t fadeValue;
  static uint32_t color;
  if (dimmerValueChangedThisIteration) {
    if (dimmerValue <= 511) {
      fadeValue = map(dimmerValue, 0, 511, 0, 255);
      color = strip.Color(fadeValue, 0, 0);
    } else if (dimmerValue <= 1022) {
      fadeValue = map(dimmerValue-511, 0, 511, 0, 255);
      color = strip.Color(fadeValue, fadeValue, fadeValue);
    }
    for (int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
  }
}

void outputAnimation() {
  static int wait = 10;
  static uint32_t white = strip.Color(255, 255, 255);
  static uint32_t black = strip.Color(0, 0, 0);

  // initialize
  for (int i=0; i<NUM_LEDS_ON_IN_ANIMATION; i++) {
    strip.setPixelColor(i, color);
  }
  // cycle forward
  for (int i=NUM_LEDS_ON_IN_ANIMATION-1; i<strip.numPixels(); i++) {
    iterate();
    strip.setPixelColor(i, color);
    for (int j=i; j>=i-NUM_LEDS_ON_IN_ANIMATION; j--) {
      strip.setPixelColor(j, color);
    }
    if (i >= NUM_LEDS_ON_IN_ANIMATION) {
      strip.setPixelColor(i-NUM_LEDS_ON_IN_ANIMATION, black);
    }
    strip.show();
    delay(wait);
  }
  // cycle back
  for (int i=strip.numPixels()-NUM_LEDS_ON_IN_ANIMATION-1; i>=0; i--) {
    iterate();
    strip.setPixelColor(i, color);
    for (int j=i; j<=i+NUM_LEDS_ON_IN_ANIMATION; j++) {
      strip.setPixelColor(j, color);
    }
    if (i < strip.numPixels()-NUM_LEDS_ON_IN_ANIMATION) {
      strip.setPixelColor(i+NUM_LEDS_ON_IN_ANIMATION, black);
    }
    strip.show();
    delay(wait);
  }
  delay(50);

  // turn off
  for (int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, black);
  }
  strip.show();
}

void iterate() {
  static int iterationsInAnimation = (strip.numPixels()-NUM_LEDS_ON_IN_ANIMATION)*4;
  static int count = 0;

  static int red = 255;
  static int green = 0;
  static int blue = 0;
  static int colorRange;
  colorRange = map(count, 0, iterationsInAnimation, 0, 1530);

  if (colorRange>=0 && colorRange<=255) {
    green = colorRange;
  } else if (colorRange>=256 && colorRange<=511) {
    red = 511-colorRange;
  } else if (colorRange>=512 && colorRange<=767) {
    blue = colorRange-512;
  } else if (colorRange>=768 && colorRange<=1023) {
    green = 1023-colorRange;
  } else if (colorRange>=1024 && colorRange<=1279) {
    red = colorRange-1024;
  } else if (colorRange>=1280 && colorRange<=1530) {
    blue = 1530-colorRange;
  }

  color = strip.Color(red, green, blue);

  if (count == iterationsInAnimation) {
    count = 0;
  } else {
    count = count + 1;
  }
}
