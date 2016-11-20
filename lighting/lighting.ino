
#define BAUD_RATE 9600

#define LOOP_DELAY_ACTIVE    10
#define LOOP_DELAY_INACTIVE 500

// Motion Sensor
// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
#define PIN_PIR  2
unsigned int lastMotionDetectionMs = 0;

// Dimmer
#define PIN_POT 0
int dimmerValue = 0;

// Light
#define PIN_LED 9
#define LIGHT_ON_MIN_DURATION 300000 // 5 minutes
bool lightOn = false;

void setup() {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(BAUD_RATE);
  Serial.println("setup() complete");
}

void loop() {
  getMotionValue();
  getDimmerValue();
  outputState();
  delayState();
}

void getMotionValue() {
  static int pirValue;
  static int duration = 0;

  pirValue = digitalRead(PIN_PIR);

  // turn light off
  if (lightOn && pirValue == LOW) {
    duration = abs(millis() - lastMotionDetectionMs);
    if (duration > LIGHT_ON_MIN_DURATION) {
      lightOn = false;
      Serial.println("Lights out.");
    }
  }

  // turn light on
  else if (pirValue == HIGH) {
    lastMotionDetectionMs = millis();
    if (!lightOn) {
      lightOn = true;
      Serial.println("Motion detected! *beep boop*");

    }
  }

}

void getDimmerValue() {
  dimmerValue = analogRead(PIN_POT);
}

/**
 * Output state to lights
 */
void outputState() {
  static int lightValue = 0;
  if (lightOn) {
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
  if (lightOn) {
    delay(LOOP_DELAY_ACTIVE);
  } else {
    delay(LOOP_DELAY_INACTIVE);
  }
}





