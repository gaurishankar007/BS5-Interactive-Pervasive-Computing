char Mymessage[8]="Hello";

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.write(Mymessage, 8);
  delay(1000);
}
