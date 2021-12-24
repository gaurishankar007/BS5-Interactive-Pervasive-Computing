char Mymessage[10];

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.readBytes(Mymessage, 10);
  Serial.println(Mymessage);
  delay(1000);
}
