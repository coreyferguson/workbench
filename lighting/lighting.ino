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

// State
bool pirActive = false; // current pir status
bool pirWasActive = false; // pir status on last program loop
bool pirActiveThisIteration = false; // pir activated on this program loop
bool pirInactiveThisIteration = false; // pir inactivated on this program loop

void setup() {
  pinMode(PIN_PIR, INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(BAUD_RATE);
  Serial.println("setup() complete");
}

void loop() {
  updateMotionState();

  if (pirActiveThisIteration) {
    Serial.println("Activated");
  } else if (pirInactiveThisIteration) {
    Serial.println("Inactived");
  }

  delay(50);
}

void updateMotionState() {
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
}
