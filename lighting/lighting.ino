
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
#define PIN_LED 9
#define LIGHT_ON_MINIMUM_DURATION 600000 // 10 minutes
bool motionDetectedPreviously = false;
bool motionDetected = false;

void setup() {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LED, OUTPUT);

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
  outputLight();
}

void outputLight() {
  static int lightValue = 0;
  if (motionDetected) {
    lightValue = map(dimmerValue, 0, 1023, 0, 255);
    analogWrite(PIN_LED, lightValue);
  } else {
    digitalWrite(PIN_LED, LOW);
  }
}

void outputAnimation() {
  Serial.println("Animation started");
  static unsigned long startTime;
  static unsigned long endTime;
  startTime = millis();

  // calculate fade-to-full delay
  static int numberOfLoops = 7;
  static int fadeToFullIncrements = 2;
  static int fadeToFullMaxValue = 255;
  static double fadeToFullIterations =
    (double) fadeToFullMaxValue /
    fadeToFullIncrements;
  static double fadeToFullIterationDelay =
    DELAY_ANIMATION /
    numberOfLoops /
    fadeToFullIterations;

  // calculate fade-to-off delay
  static int fadeToOffIncrements = 2;
  static int fadeToOffMaxValue = 255;
  static double fadeToOffIterations =
    (double) fadeToOffMaxValue /
    fadeToOffIncrements;
  static double fadeToOffIterationDelay =
    DELAY_ANIMATION /
    numberOfLoops /
    fadeToOffIterations;

  // calculate fade-to-dimmer delay
  static int fadeToDimmerIncrements = 2;
  static double fadeToDimmerIterations;
  static int lightValue = 0;
  lightValue = map(dimmerValue, 0, 1023, fadeToDimmerIncrements, 255);
  fadeToDimmerIterations = (double) lightValue / fadeToDimmerIncrements;
  static double fadeToDimmerIterationDelay =
    DELAY_ANIMATION /
    numberOfLoops /
    fadeToDimmerIterations;

  // fade to full brightness
  analogWrite(PIN_LED, 0);
  for (int i=0; i<=fadeToFullMaxValue; i+=fadeToFullIncrements) {
    analogWrite(PIN_LED, i);
    delay(fadeToFullIterationDelay);
  }

  // fade to off
  for (int i=fadeToOffMaxValue; i>=0; i-=fadeToOffIncrements) {
    analogWrite(PIN_LED, i);
    delay(fadeToOffIterationDelay);
  }

  // fade to full brightness
  for (int i=0; i<=fadeToFullMaxValue; i+=fadeToFullIncrements) {
    analogWrite(PIN_LED, i);
    delay(fadeToFullIterationDelay);
  }

  // fade to off
  for (int i=fadeToOffMaxValue; i>=0; i-=fadeToOffIncrements) {
    analogWrite(PIN_LED, i);
    delay(fadeToOffIterationDelay);
  }

  // fade to full brightness
  for (int i=0; i<=fadeToFullMaxValue; i+=fadeToFullIncrements) {
    analogWrite(PIN_LED, i);
    delay(fadeToFullIterationDelay);
  }

  // fade to off
  for (int i=fadeToOffMaxValue; i>=0; i-=fadeToOffIncrements) {
    analogWrite(PIN_LED, i);
    delay(fadeToOffIterationDelay);
  }

  // fade to dimmer value
  for (int i=0; i<=lightValue; i+=fadeToDimmerIncrements) {
    analogWrite(PIN_LED, i);
    delay(fadeToDimmerIterationDelay);
  }

  endTime = millis();
  Serial.println("Animation end in " + String(endTime - startTime) + "ms");
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
