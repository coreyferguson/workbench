
// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
int pirPin = 2;
int pirValue;
int lastPirValue = LOW;

// Baud rate in Serial Monitor
int baudRate = 9600;

void setup() {
  pinMode(pirPin, INPUT);

  Serial.begin(baudRate);

  Serial.println();
  Serial.println("setup() complete");
}

void loop() {
  pirValue = digitalRead(pirPin);
  if (pirValue == LOW && lastPirValue == HIGH) {
    Serial.println("Waiting...");
  }
  else if (pirValue == HIGH && lastPirValue == LOW) {
    Serial.println("Detection! *beep boop*");
  }
  lastPirValue = pirValue;
  delay(100);
}

