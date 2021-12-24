int ledPin = 13;
int pirPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int pirState = digitalRead(pirPin);
  if(pirState==1) {
    digitalWrite(ledPin, HIGH);
    Serial.print("Detected: LED ON");
    Serial.println("");
  }
  if(pirState==0){
    digitalWrite(ledPin, LOW);
    Serial.println("Detection Lost: LED OFF");
  }
  delay(250);
}
