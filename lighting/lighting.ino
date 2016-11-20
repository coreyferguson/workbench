
#define BAUD_RATE 9600

#define LOOP_DELAY_ACTIVE    10
#define LOOP_DELAY_INACTIVE 500

// Motion Sensor
// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
#define PIN_PIR  2

// Dimmer
#define PIN_POT 0
int dimmerValue = 0;

// Light
#define PIN_LED 9
#define LIGHT_ON_MIN_DURATION 3000
bool motionDetectedPreviously = false;
bool motionDetected = false;

void setup() {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(BAUD_RATE);
  Serial.println("setup() complete");
}

void loop() {
  updateMotionState();
  updateDimmerState();
  printState();
  outputState();
  delayState();
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
    if (duration > LIGHT_ON_MIN_DURATION) {
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

void printState() {
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
void outputState() {
  static int lightValue = 0;
  if (motionDetected) {
    lightValue = map(dimmerValue, 0, 1023, 0, 255);
    analogWrite(PIN_LED, lightValue);
  } else {
    digitalWrite(PIN_LED, LOW);
  }
}

/**
 * Delay application depending on state.
 */
void delayState() {
  if (motionDetected) {
    delay(LOOP_DELAY_ACTIVE);
  } else {
    delay(LOOP_DELAY_INACTIVE);
  }
}




