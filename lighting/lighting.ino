
// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
int pirPin = 2;
int pirValue;

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
  if (pirValue == LOW) {
    Serial.println("Detection! *beep boop*");
  }
  else {
    Serial.println("Waiting...");
  }
  delay(1000);
}

