void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  // pinMode(4, OUTPUT); Digital
  // pinMode(A0, INPUT); Analog
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  delay(50); //Waiting in milisecond    
  digitalWrite(13, LOW);
  delay(50); //Waiting
}
