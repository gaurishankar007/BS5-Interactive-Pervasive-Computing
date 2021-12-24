// VVI MQ2 Gas Sensor
int redLed = 13;
int greenLed = 12;
//int buzzer = 11; // If buzzer is present
int smoke = A0;
int thressVal = 60;

void setup() {
  pinMode(redLed, OUTPUT); 
  pinMode(greenLed, OUTPUT);
  //pinMode(buzzer, OUTPUT); // If buzzer is present
  pinMode(smoke, INPUT);
  Serial.begin(9600);
}

void loop() {
  int analogVal = analogRead(smoke);
  Serial.print("Pin A0: ");
  Serial.println(analogVal);
  // Checks if it has reached the threshold value
  if (analogVal > thressVal)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    Serial.println("Smoke Detected.");
    //tone(buzzer, 1000, 200); // If buzzer is present
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    Serial.println("Smoke not Detected.");
    //noTone(buzzer); // If buzzer is present
  }
  delay(250);
}
