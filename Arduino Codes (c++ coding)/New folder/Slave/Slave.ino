int LED = 13;
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("START");
}

void loop() {
  if(Serial.available()) {
    char number = Serial.read();
    if (number=='0'){
      digitalWrite(LED, LOW);
    }
    if (number=='1') {
      digitalWrite(LED, HIGH);
    }
    Serial.println(number);
  }
}
