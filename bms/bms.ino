// ESP8266WiFi for esp8266 - Version: Latest
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>

#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>


#include <ESP8266WiFi.h>

String apiKey = "O06YKZM7IRJIXQNY";
const char* ssid = "Redmi Note 9 Pro";  // Enter your WiFi Network's SSID
const char* pass = "44444444";          // Enter your WiFi Network's Password
const char* server = "api.thingspeak.com";

int analogInPin = A0;      // Analog input pin
int sensorValue;           // Analog Output of Sensor
float calibration = 0.36;  // Check Battery voltage using multimeter & add/subtract the value
int bat_percentage;

WiFiClient client;

void setup() {
  Serial.begin(460800);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("*");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  sensorValue = analogRead(analogInPin);
  float voltage = (((sensorValue * 4.2) / 1024)  - calibration); 

  bat_percentage = mapfloat(voltage, 2.5, 4.2, 0, 100);  //2.5V as Battery Cut off Voltage & 4.2V as Maximum Voltage

  if (bat_percentage >= 100) {
    bat_percentage = 100;
  }
  if (bat_percentage <= 0) {
    bat_percentage = 1;
  }

  Serial.print("Analog Value = ");
  Serial.print(sensorValue);
  Serial.print("\t Output Voltage = ");
  Serial.print(voltage);
  Serial.print("\t Battery Percentage = ");
  Serial.println(bat_percentage);
  delay(1000);

  if (client.connect(server, 80)) {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(voltage);
    postStr += "&field2=";
    postStr += String(bat_percentage);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    delay(1);
    client.print("Host: api.thingspeak.com\n");
    delay(1);
    client.print("Connection: close\n");
    delay(1);
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    delay(1);
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    delay(1);
    client.print("Content-Length: ");
    delay(1);
    client.print(postStr.length());
    delay(1);
    client.print("\n\n");
    delay(1);
    client.print(postStr);
    delay(1);
  }
  client.stop();
  Serial.println("Sending....");
  delay(10);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}