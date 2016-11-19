
#define MS_DELAY_INACTIVE 500   // 1/2 second
#define MS_DELAY_ACTIVE   1000 //  1 seconds

// Component: HC-SR501
// PIR = Passive Infrared sensor
// Digital output
int pirPin = 2;
int pirValue;
int lastPirValue = LOW;

// Lights
int ledPin = 13;

// Baud rate in Serial Monitor
int baudRate = 9600;

// Milliseconds since last detection
unsigned long millisLastDetected = 0;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(baudRate);

  Serial.println();
  Serial.println("setup() complete");
}

void loop() {
  // detect motion
  pirValue = digitalRead(pirPin);
  if (pirValue == LOW) {
    Serial.println("Waiting...");
    digitalWrite(ledPin, LOW);
    delay(MS_DELAY_INACTIVE);
  } else {
    Serial.println("Detection! *beep boop*");
    digitalWrite(ledPin, HIGH);
    delay(MS_DELAY_ACTIVE);
  }

}



