#include <SoftwareSerial.h>
SoftwareSerial nodemcu(6, 7); // Connect Arduino digital pin 6 to RX and 7 to TX of NodeMCU
SoftwareSerial sim900();

// Pins
int ledO=11, ledR=12, ledP=13;
int bzr=8;
int gas = A0, flame = A1, temp = A2;

// Threshold values
int gasThres = 250, flameThres = 100;

// Sensor information
String senVal="";

void setup() {
  // OUTPUT Pinmode
  pinMode(ledO, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledP, OUTPUT);
  pinMode(bzr, OUTPUT);

  // INPUT Pinmode
  pinMode(gas, INPUT);
  pinMode(flame, INPUT);
  pinMode(temp, INPUT);

  // Start Value
  nodemcu.begin(9600);
  Serial.begin(9600);
  Serial.println("<<--Reading Sensors-->>");
}

void loop() {
  // Reading Data
  int gasRead = analogRead(gas);
  int flameRead = analogRead(flame);
  int tempRead = analogRead(temp);
  int newTemp = tempRead/2.048; // Coverting temperature analog value to centigrade/celsius 

  // Sending Data to NodeMCU    
  String devider = ":";
  senVal = gasRead+devider+flameRead+devider+tempRead+";";
  nodemcu.println(senVal);
  senVal="";

  // Controlling LED and BUZZER
  if(gasRead > gasThres || flameRead < flameThres){  
    digitalWrite(ledP, LOW);    
    tone(bzr, 500, 300);      
    if(gasRead > gasThres) {
      digitalWrite(ledO, HIGH);  
    }
    else {
      digitalWrite(ledO, LOW);
    }
  
    if(flameRead < flameThres) {
      digitalWrite(ledR, HIGH);   
    }
    else {
      digitalWrite(ledR, LOW);    
    } 
  }
  else {
    digitalWrite(ledO, LOW);  
    digitalWrite(ledR, LOW);  
    digitalWrite(ledP, HIGH);
    noTone(bzr);  
  }  

  // Printing Sesors data to Serial Monitor 
  Serial.print("Gas:- ");
  Serial.print(gasRead);
  Serial.print(", Flame: ");
  Serial.print(flameRead); 
  Serial.print(", Temperature/Celcius:- ");
  Serial.print(newTemp);
  Serial.println("");
  
  delay(1500);
}
