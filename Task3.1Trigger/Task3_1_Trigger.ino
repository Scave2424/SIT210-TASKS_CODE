#include <WiFiNINA.h>
#include <WiFiSSLClient.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <BH1750.h>

// Wi-Fi details

const char* wifiSSID = "xyz";
const char* wifiPassword = "xyz";

// HiveMQ details

const char* mqttServer =
  "2a44e3ab0d05405ba327b654757406b5.s1.eu.hivemq.cloud";

const int mqttPort = 8883;

const char* mqttUsername = "Scave";
const char* mqttPassword = "xyz";

// MQTT topic
const char* mqttTopic = "sit210/terrarium/light";

// Objects

WiFiSSLClient wifiClient;
PubSubClient mqttClient(wifiClient);

BH1750 lightMeter;

// Setup

void setup() {

  Serial.begin(115200);

  // Start I2C
  Wire.begin();

  // Start BH1750
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 started successfully");
  }
  else {
    Serial.println("Error starting BH1750");
  }

  // Connect to Wi-Fi
  connectWiFi();

  // Set HiveMQ broker
  mqttClient.setServer(mqttServer, mqttPort);

  // Connect to HiveMQ
  connectMQTT();
}

// Main loop

void loop() {

  // Check MQTT connection
  if (!mqttClient.connected()) {
    connectMQTT();
  }

  mqttClient.loop();

  // Read light level
  float lux = lightMeter.readLightLevel();

  // Print lux to Serial Monitor
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");

  // Convert lux number to text
  char message[20];
  snprintf(message, sizeof(message), "%.2f", lux);

  // Publish lux value to HiveMQ
  mqttClient.publish(mqttTopic, message);

  Serial.println("Published to HiveMQ");

  delay(5000);
}

// Connect Wi-Fi

void connectWiFi() {

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {

    WiFi.begin(wifiSSID, wifiPassword);

    delay(5000);

    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Connect MQTT

void connectMQTT() {

  Serial.print("Connecting to HiveMQ...");

  while (!mqttClient.connected()) {

    String clientID = "Nano33IoT-" + String(random(0xffff), HEX);

    if (mqttClient.connect(
          clientID.c_str(),
          mqttUsername,
          mqttPassword)) {

      Serial.println("connected!");
    }
    else {

      Serial.print("failed, state = ");
      Serial.println(mqttClient.state());

      delay(5000);
    }
  }
}