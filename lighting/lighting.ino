
#define MS_DELAY_INACTIVE 50   // 1/2 second
#define MS_DELAY_ACTIVE   1000 //  1 seconds

#define BAUD_RATE 9600

#define LOOP_DELAY_ACTIVE    10
#define LOOP_DELAY_INACTIVE 500

// Motion Sensor
// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
#define PIN_PIR  2
int dimmerValue = 0;

// Dimmer
#define PIN_POT 0

// Light
#define PIN_LED 9
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
  printState();
  outputState();
  delayState();
}

void getMotionValue() {
  static int pirValue;
  static int lastPirValue = LOW;
  pirValue = digitalRead(PIN_PIR);
  if (pirValue == LOW) {
    lightOn = false;
  } else if (pirValue == HIGH) {
    lightOn = true;
  }
}

void getDimmerValue() {
  dimmerValue = analogRead(PIN_POT);
}

/**
 * Print state to serial monitor.
 */
void printState() {
  if (lightOn) {
    Serial.println("Lights on");
  } else {
    Serial.println("Lights off");
  }
  Serial.println(dimmerValue, DEC);
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



