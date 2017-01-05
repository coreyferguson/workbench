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
bool motionDetectedPreviously = false;
bool motionDetected = false;

void setup() {
  pinMode(PIN_PIR, INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(BAUD_RATE);
  Serial.println("setup() complete");
}

void loop() {
  updateMotionState();
  printToSerial();
  output();
  delay(1000);
}

void updateMotionState() {
  motionDetectedPreviously = motionDetected;
  motionDetected = digitalRead(PIN_PIR);
}

void printToSerial() {
  static unsigned long startTime;
  static unsigned long endTime;
  static unsigned long duration;

  if (motionDetectedPreviously != motionDetected) {
    if (motionDetected) {
      Serial.println("Motion detected! *beep boop*");
      startTime = millis();
    } else {
      Serial.println("Waiting for movement...");
      endTime = millis();
      duration = (endTime - startTime) / 1000;
      Serial.println("Active for " + String(duration, DEC) + " seconds");
    }
  }
}

/**
 * Output state to lights
 */
void output() {
  static int lightValue = 0;
  static uint32_t color;
  if (motionDetected) {
    color = strip.Color(255, 255, 255);
  } else {
    color = strip.Color(0,0,0);
  }
  for (int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
