#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

char ssid[] = "xyz";
char pass[] = "1234";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
const int port = 1883;

// LED pins
const int led1 = 6;
const int led2 = 7;

// Ultrasonic sensor pins
const int trigPin = 2;
const int echoPin = 3;

// Handles incoming MQTT messages
void onMessageReceived(int messageSize) {

  String topic = mqttClient.messageTopic();

  Serial.print("MQTT message received on: ");
  Serial.println(topic);

  Serial.print("Message: ");

  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }

  Serial.println();

  // Turns both LEDs on for a wave
  if (topic == "ES/Wave") {

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);

    Serial.println("ES/Wave received -> LEDs ON");
  }

  // Turns both LEDs off for a pat
  if (topic == "ES/Pat") {

    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);

    Serial.println("ES/Pat received -> LEDs OFF");
  }
}

// Measures distance using the ultrasonic sensor
float getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2;
}

// Publishes a wave message
void publishWave() {

  Serial.println("WAVE DETECTED");

  mqttClient.beginMessage("ES/Wave");
  mqttClient.print("Jasmeet");
  mqttClient.endMessage();

  Serial.println("Published to ES/Wave");
}

// Publishes a pat message
void publishPat() {

  Serial.println("PAT DETECTED");

  mqttClient.beginMessage("ES/Pat");
  mqttClient.print("Jasmeet");
  mqttClient.endMessage();

  Serial.println("Published to ES/Pat");
}

void setup() {

  Serial.begin(9600);

  while (!Serial) {
    ;
  }

  // Set LED pins as outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  // Set ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {

    Serial.println("Trying again...");
    delay(5000);
  }

  Serial.println("Wi-Fi connected!");

  // Connect to MQTT broker
  mqttClient.setId("Nano33IoT-Jasmeet");

  mqttClient.onMessage(onMessageReceived);

  Serial.println("Connecting to MQTT...");

  if (!mqttClient.connect(broker, port)) {

    Serial.print("MQTT connection failed. Error = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("MQTT connected!");

  // Subscribe to MQTT topics
  mqttClient.subscribe("ES/Wave");
  mqttClient.subscribe("ES/Pat");

  Serial.println("Subscribed to ES/Wave");
  Serial.println("Subscribed to ES/Pat");

  Serial.println("System ready!");
}

void loop() {

  // Keep the MQTT connection active
  mqttClient.poll();

  // Read the current distance
  float distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Detect a pat within 10 cm
  if (distance > 0 && distance <= 10) {

    publishPat();

    delay(2000);
  }

  // Detect a wave between 20 and 50 cm
  else if (distance > 20 && distance <= 50) {

    publishWave();

    delay(2000);
  }

  delay(200);
}