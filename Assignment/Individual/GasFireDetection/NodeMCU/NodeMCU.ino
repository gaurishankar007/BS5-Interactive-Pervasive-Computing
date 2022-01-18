#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

SoftwareSerial nodemcu(D7, D6);

// Library for spliting string and reading the required value
#include <iostream>
#include <string>
using namespace std;

// Defining Blynk Auth, and Wifi SSID and Password for NodeMCU
char auth[] = "iYqjF5GfliSVeQ-lxog6BBUvLznxrwBk";
char ssid[] = "ritalimbu_wlink";
char pass[] = "1a2b3c4d5e6f";

// Temperature values
int curTemp, maxTemp, minTemp;

// Sensor Information
String senInfo="";

void setup() { 
  Serial.begin(9600);
  nodemcu.begin(9600);
  Blynk.begin(auth, ssid, pass);
  Blynk.syncAll();
}

void loop() {
  Blynk.run();
  
  // Reading sensor values send from Arduino 
  if(nodemcu.available()) {
    senInfo = nodemcu.readStringUntil(';');
    Serial.println(senInfo);
  } 
  else {
    Serial.println("Communication Lost");
  }
  int gasRead = getSenVal(0);
  int flameRead = getSenVal(1);
  int tempRead = getSenVal(2);
  int newTemp = tempRead/2.048;   
  senInfo="";   
  
  // Updating Temperature values
  curTemp = newTemp;
  maxTemp = newTemp;
  minTemp = newTemp;
  if(newTemp>maxTemp){
    maxTemp = newTemp;
  } 
  if(newTemp<minTemp){
    minTemp = newTemp;
  }  
  
  Blynk.virtualWrite(V0, gasRead);
  Blynk.virtualWrite(V1, flameRead);
  Blynk.virtualWrite(V2, tempRead);  
  Blynk.virtualWrite(V3, curTemp);  
  Blynk.virtualWrite(V4, maxTemp);  
  Blynk.virtualWrite(V5, minTemp);

  delay(200);
}

int getSenVal(int senIndex) { 
  int infoLen = senInfo.length();
  int senVal = 0;
  char devider = ':';
  
  if(senIndex > -1) {
    int i = 0;
    int j = -1;
    int startIndex = 0;
    int endIndex = 0;
    String val = "";    
    
    while(i < infoLen) {
      if(senInfo.charAt(i)==devider || i==(infoLen-1)) {
        j++;
        if(j==senIndex) {
          if(i==(infoLen-1)) {
            endIndex = i+1;  
          }
          else {
            endIndex = i;  
          }        
          val = senInfo.substring(startIndex, endIndex);
          i = i + infoLen;
        }
        else {
          startIndex = i+1;
        }
      }
      i++; 
    } 
    senVal = val.toInt();
  }  
  return senVal;
}
