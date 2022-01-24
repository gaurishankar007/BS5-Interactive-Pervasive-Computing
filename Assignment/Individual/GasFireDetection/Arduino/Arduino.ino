#include <SoftwareSerial.h>
SoftwareSerial nodemcu(6, 7); // 6=Rx & 7=Tx
SoftwareSerial SIM900(4, 5);

// Pins
int ledO=11, ledR=12, ledP=13;
int bzr=8;
int gas=A0, flame=A1, temp=A2;

// Threshold values
int gasThres=250, flameThres=100;

// Sensor information
String senVal="";
bool gasAlert=false;
bool flameAlert=false;

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
  SIM900.begin(19200);
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
    tone(bzr, 200, 300); 
       
    if(gasRead > gasThres) {
      digitalWrite(ledO, HIGH);  
      if(gasAlert==false) {
        sendMessage("gas");   
        gasAlert=true;         
      }   
    }
    else {
      digitalWrite(ledO, LOW);
    }
  
    if(flameRead < flameThres) {
      digitalWrite(ledR, HIGH);   
      if(flameAlert==false) {
        sendMessage("flame");    
        flameAlert=true;                 
      }    
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
    // Avoiding multiple message sending 
    gasAlert=false;        
    flameAlert=false;       
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

void sendMessage(String sensor) {
  String message = "Gas or Fire Detected.";
  if(sensor == "gas") {
    message="Gas Detection Alert :(. Check the room or maybe you can run away and save your life.";
  }
  if(sensor == "flame") {
    message="Fire Detection Alert :(. Check the room or maybe you can run away and save your life.";    
  }  
  
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  
  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+9779816349292\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}
