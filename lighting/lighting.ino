#define BAUD_RATE 9600

// Motion Sensor
// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
#define PIN_PIR A2

#define PIN_LED 13

void setup() {
  pinMode(PIN_PIR, INPUT);

  Serial.begin(BAUD_RATE);
  Serial.println("setup() complete");
}

void loop() {
  static bool pirActive;
  pirActive = digitalRead(PIN_PIR);
  Serial.println(pirActive, DEC);
  if (pirActive) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }
  delay(500);
}
