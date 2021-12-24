int potentiometerPin = A0;
int ledPin = 13;
int readVal;
float  voltage;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(potentiometerPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  readVal = analogRead(potentiometerPin);
  voltage = (5./1023.)*readVal;
  if(voltage>=3) {
    digitalWrite(ledPin, HIGH);
    delay(300);
  }
  if(voltage<3) {
    digitalWrite(ledPin, LOW);
    delay(300);
  }
  Serial.print("Potentiometer Resistance value: ");
  Serial.println(readVal);
  Serial.print("Voltage got by led: ");
  Serial.println(voltage);
  delay(500);
}
