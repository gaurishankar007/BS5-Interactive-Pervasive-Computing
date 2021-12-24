#include <LiquidCrystal.h>
LiquidCrystal LCD(12,11,5,4,3,2);
int triPin = 8;
int echPin = 7;
int ledPin = 13;

void setup(){
  pinMode(triPin, OUTPUT);
  pinMode(echPin, INPUT);

  LCD.begin(16,2);
  LCD.setCursor(0,0);
  LCD.print("Object Distance: ");
}

void loop(){
  long duration, distance;
  digitalWrite(triPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triPin, LOW);

  duration = pulseIn(echPin, HIGH);
  distance = 0.034*(duration/2);
  if(distance <= 50){
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }

  LCD.setCursor(0,1);
  LCD.print(distance); 
  LCD.setCursor(3,1);
  LCD.print(" cm");
  delay(250);  
}
 
