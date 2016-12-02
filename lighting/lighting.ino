#include <Adafruit_NeoPixel.h>


#define BAUD_RATE 9600

#define DELAY_LOOP_ACTIVE    10
#define DELAY_LOOP_INACTIVE 500
#define DELAY_ANIMATION    1500

// Motion Sensor
// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
#define PIN_PIR  2

// Dimmer
#define PIN_POT 0
int dimmerValue = 0;

// Light
#define PIN_LED 6
#define LIGHT_ON_MINIMUM_DURATION 300000 // 5 minutes
bool motionDetectedPreviously = false;
bool motionDetected = false;
#define NUM_LEDS_ON_IN_ANIMATION 15
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN_LED, NEO_GRBW + NEO_KHZ800);
static uint32_t color = strip.Color(255,0,0);

void setup() {
  pinMode(PIN_PIR, INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(BAUD_RATE);
  Serial.println("setup() complete");
}

void loop() {
  // Update state from input
  updateMotionState();
  updateDimmerState();

  // Logging
  printToSerial();
  output();

  // Loop delay
  sleep();
}

void updateMotionState() {
  static int pirValue;
  static int duration = 0;
  static unsigned int lastMotionDetectionMs = 0;

  motionDetectedPreviously = motionDetected;

  // update motion state based on sensor value
  pirValue = digitalRead(PIN_PIR);
  if (motionDetected && pirValue == LOW) {
    duration = abs(millis() - lastMotionDetectionMs);
    if (duration > LIGHT_ON_MINIMUM_DURATION) {
      motionDetected = false;
    }
  } else if (pirValue == HIGH) {
    lastMotionDetectionMs = millis();
    motionDetected = true;
  }
}

void updateDimmerState() {
  dimmerValue = analogRead(PIN_POT);
}

void printToSerial() {
  if (motionDetectedPreviously != motionDetected) {
    if (motionDetected) {
      Serial.println("Motion detected! *beep boop*");
    } else {
      Serial.println("Waiting for movement...");
    }
  }
}

/**
 * Output state to lights
 */
void output() {
  if (motionDetectedPreviously != motionDetected) {
    if (motionDetected) {
      outputAnimation();
    }
  }

  // "party mode" when dimmer turned all the way up
  if (dimmerValue == 1023) {
    outputAnimation();
  }

  // normal behavior
  else {
    outputLight();
  }
}

void outputLight() {
  static int lightValue = 0;
  static uint32_t color;
  if (motionDetected) {
    lightValue = map(dimmerValue, 0, 1023, 0, 255);
    color = strip.Color(lightValue, lightValue, lightValue);
  } else {
    color = strip.Color(0,0,0);
  }
  for (int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
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
  // cycle forward
  for (int i=NUM_LEDS_ON_IN_ANIMATION; i<strip.numPixels(); i++) {
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

/**
 * Delay application depending on state.
 */
void sleep() {
  if (motionDetected) {
    delay(DELAY_LOOP_ACTIVE);
  } else {
    delay(DELAY_LOOP_INACTIVE);
  }
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

