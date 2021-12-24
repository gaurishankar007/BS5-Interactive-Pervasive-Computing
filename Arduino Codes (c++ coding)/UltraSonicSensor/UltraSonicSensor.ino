const int triPin = 8;
const int echPin = 9;
const int ledPin = 13;

int duration = 0;
int distance = 0;

void setup(){
  pinMode(triPin, OUTPUT);
  pinMode(echPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  digitalWrite(triPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triPin, LOW);

  duration = pulseIn(echPin, HIGH);
  distance = duration*(0.034/2);
  Serial.print("Distance is: ");
  Serial.println(distance);

  if(distance <= 50){
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
  delay(500);
}
