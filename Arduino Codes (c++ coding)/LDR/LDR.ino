int ledPin = 13;
int ldrPin = A0;
int ldrVal;

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  ldrVal = analogRead(ldrPin);
  if (ldrVal<=300){
    digitalWrite(ledPin, HIGH);
    Serial.print("It's Bright. Turn off LED: ");
    Serial.println(ldrVal);
    delay(500);
  }
  else {
    digitalWrite(ledPin, LOW);
    Serial.println("It's DARK. Turn on LED: ");
    Serial.print (ldrVal);
    delay(500);
  }
}
