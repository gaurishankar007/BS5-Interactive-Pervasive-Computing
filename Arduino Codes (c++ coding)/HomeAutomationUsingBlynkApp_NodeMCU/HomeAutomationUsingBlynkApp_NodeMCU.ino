#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "WyLhZ0jZKaDosv5AZR1yV1vXzmTGvWJJ";
char ssid[] = "sarkar";
char pass[] = "123456789";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  Blynk.syncAll(); 
}

void loop()
{
  Blynk.run();
}
