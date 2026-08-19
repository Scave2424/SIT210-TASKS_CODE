/*
  Task 2.1P - Sending Temperature, Humidity and Light Data to ThingSpeak
  Board: Arduino Nano 33 IoT
  Sensors: DHT11/22 (temperature + humidity), BH1750 (I2C light sensor)

  Channel fields:
  Field 1 = Temperature
  Field 2 = Humidity
  Field 3 = Light
*/

#include <WiFiNINA.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>       
#include "secrets.h"      // holds WiFi + ThingSpeak credentials, kept separate from this file
#include "ThingSpeak.h"  

//  WiFi credentials, pulled from secrets.h 
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

//  ThingSpeak settings, pulled from secrets.h 
unsigned long channelID = SECRET_CH_ID;
const char* writeAPIKey = SECRET_WRITE_APIKEY;

//  Pin setup 
#define DHTPIN 3          // DHT data pin wiring to D3
#define DHTTYPE DHT11     

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;        // uses default I2C pins: SDA -> A4, SCL -> A5
WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();

  Wire.begin();          // starts I2C on A4/A5
  lightMeter.begin();    // starts the BH1750 sensor

  connectToWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  float temperature = readTemperature();
  float humidity = readHumidity();
  float lightLevel = readLight();

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C | Humidity: ");
  Serial.print(humidity);
  Serial.print(" % | Light: ");
  Serial.print(lightLevel);
  Serial.println(" lx");

  sendToThingSpeak(temperature, humidity, lightLevel);

  delay(30000); // send every 30 seconds, as required by the task
}

// Connects to WiFi before anything else runs
void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected.");
}

// Reads temperature from the DHT sensor
float readTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Failed to read temperature.");
    return 0;
  }
  return temp;
}

// Reads humidity from the DHT sensor
float readHumidity() {
  float hum = dht.readHumidity();
  if (isnan(hum)) {
    Serial.println("Failed to read humidity.");
    return 0;
  }
  return hum;
}

// Reads light level in lux from the BH1750 over I2C
float readLight() {
  return lightMeter.readLightLevel();
}

// Packages all three readings into ThingSpeak fields and pushes them
void sendToThingSpeak(float temp, float hum, float light) {
  ThingSpeak.setField(1, temp);   // Field 1 = Temperature
  ThingSpeak.setField(2, hum);    // Field 2 = Humidity
  ThingSpeak.setField(3, light);  // Field 3 = Light

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (status == 200) {
    Serial.println("Data sent to ThingSpeak.");
  } else {
    Serial.println("Error sending data. HTTP code: " + String(status));
  }
}