#include <Adafruit_NeoPixel.h>

#define BAUD_RATE 9600

// Motion Sensor
// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
#define PIN_PIR A2

// Light
#define PIN_LED 6
#define LIGHT_ON_SECONDS 30
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN_LED, NEO_GRBW + NEO_KHZ800);
uint32_t white = strip.Color(255, 255, 255);
uint32_t black = strip.Color(0, 0, 0);

// State
bool pirActive = false; // current pir status
bool pirWasActive = false; // pir status on last program loop
bool pirActiveThisIteration = false; // pir activated on this program loop
bool pirInactiveThisIteration = false; // pir inactivated on this program loop
unsigned long lightActivatedMillis;
bool lightOn = false;

void setup() {
  pinMode(PIN_PIR, INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(BAUD_RATE);
  Serial.println("setup() complete");
}

void loop() {
  updateState();

  if (pirActiveThisIteration) {
    Serial.println("Activated\t" + String(millis()));
  } else if (pirInactiveThisIteration) {
    Serial.println("Inactived\t" + String(millis()));
  }

  output();

  delay(5);
}

void updateState() {
  // PIR
  pirWasActive = pirActive;
  pirActive = digitalRead(PIN_PIR);
  if (pirActive && !pirWasActive) {
    pirActiveThisIteration = true;
    pirInactiveThisIteration = false;
  } else if (!pirActive && pirWasActive) {
    pirActiveThisIteration = false;
    pirInactiveThisIteration = true;
  } else {
    pirActiveThisIteration = false;
    pirInactiveThisIteration = false;
  }
  // Light
  if (pirActiveThisIteration) {
    lightActivatedMillis = millis();
    lightOn = true;
  } else if (millis()-lightActivatedMillis > LIGHT_ON_SECONDS*1000) {
    lightOn = false;
  }

}

void output() {
  if (lightOn) {
    for (int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, white);
    }
    strip.show();
  } else {
    for (int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, black);
    }
    strip.show();
  }
}

