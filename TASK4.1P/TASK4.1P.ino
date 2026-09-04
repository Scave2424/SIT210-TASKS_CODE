#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

const int pirPin = 2;
const int buttonPin = 3;
const int led1 = 4;
const int led2 = 5;

void motionDetected() {
  digitalWrite(led1, !digitalRead(led1));  // Toggle LED1
  digitalWrite(led2, !digitalRead(led2));  // Toggle LED2
  Serial.println("Motion detected: Lights toggled");
}

void buttonPressed() {
  digitalWrite(led1, !digitalRead(led1));  // Toggle LED1
  digitalWrite(led2, !digitalRead(led2));  // Toggle LED2
  Serial.println("Button pressed: Lights toggled");
}

void displayLux() {
  float lux = lightMeter.readLightLevel();  // Read light level
  Serial.print("Light level: ");
  Serial.print(lux);
  Serial.println(" lux");
}

void setup() {
  Serial.begin(9600);

  // Set pins
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Set interrupts
  attachInterrupt(digitalPinToInterrupt(pirPin), motionDetected, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, RISING);

  // Start BH1750
  Wire.begin();
  lightMeter.begin();

  Serial.println("System ready");
}

void loop() {
  displayLux();       // Display lux reading
  motionDetected();   // Check motion
  buttonPressed();    // Check button
  delay(1000);
}