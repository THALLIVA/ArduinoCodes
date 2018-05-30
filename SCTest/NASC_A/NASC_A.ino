#include <ArduinoJson.h>
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

/*
#include <SoftwareSerial.h>
SoftwareSerial ss(11,10);
*/
#include "dht11.h"
dht11 dht;
int temp;


void setup()
{
	Serial.begin(9600);
  //ss.begin(9600);
}

void loop()
{
//  delay(5000);
//  Serial.print(1);
//  delay(5000);
//  Serial.print("2");
//  delay(5000);
//  Serial.print(1.44);
//  delay(5000);
//  Serial.print("three");
//  delay(5000);

  delay(5000);
  temp = dht.read(3);
  root["topic"] = "P1/temperature";
  //root["time"] = millis();
  root["value"] = dht.temperature;
  root.printTo(Serial);
 
  delay(5000);
  root["topic"] = "G1/humidity";
  //root["time"] = millis();
  root["value"] = dht.humidity;
  root.printTo(Serial);
  
  delay(5000);
  root["topic"] = "G1/air";
  //root["time"] = millis();
  root["value"] = analogRead(A2);
  root.printTo(Serial);

  delay(5000);
  root["topic"] = "P1/moisture";
  //root["time"] = millis();
  root["value"] = analogRead(A1);
  root.printTo(Serial);

  delay(5000);
  root["topic"] = "light";
  //root["time"] = millis();
  root["value"] = analogRead(A0);
  root.printTo(Serial);
  /*
  delay(5000);
  ss.print(1);
  delay(5000);
  ss.print("2");
  delay(5000);
  ss.print(1.44);
  delay(5000);
  ss.print("three");
  delay(5000);
  temp = dht.read(12);
  ss.print(dht.temperature);
  delay(5000);
  ss.print(dht.humidity);
  delay(5000);
  ss.print(analogRead(A0));
  */
}
