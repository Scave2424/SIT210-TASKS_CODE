#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

// Pin definitions
const int pirPin = 2;
const int buttonPin = 3;
const int led1 = 4;
const int led2 = 5;

// Interrupt flags
volatile bool motionFlag = false;
volatile bool buttonFlag = false;

// Light threshold
const float darkThreshold = 50.0;


// PIR interrupt service routine
void motionDetected() {
  motionFlag = true;
}


// Button interrupt service routine
void buttonPressed() {
  buttonFlag = true;
}


// Toggle both LEDs
void toggleLights() {
  digitalWrite(led1, !digitalRead(led1));
  digitalWrite(led2, !digitalRead(led2));
}


// Read BH1750 light level
float readLux() {

  float lux = lightMeter.readLightLevel();

  Serial.print("Light level: ");
  Serial.print(lux);
  Serial.println(" lux");

  return lux;
}


void setup() {

  Serial.begin(9600);

  // PIR sensor
  pinMode(pirPin, INPUT);

  // Push button using internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // LEDs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  // Start BH1750
  Wire.begin();

  if (lightMeter.begin()) {
    Serial.println("BH1750 started successfully");
  }
  else {
    Serial.println("BH1750 error");
  }

  // PIR interrupt
  attachInterrupt(
    digitalPinToInterrupt(pirPin),
    motionDetected,
    RISING
  );

  // Button interrupt
  // Button pin becomes LOW when pressed
  attachInterrupt(
    digitalPinToInterrupt(buttonPin),
    buttonPressed,
    FALLING
  );

  Serial.println("System ready");
}


void loop() {
  readLux();
  // Motion detected
  if (motionFlag) {

    motionFlag = false;

    float lux = readLux();

    Serial.println("Motion detected");

    // Lights operate only when dark
    if (lux < darkThreshold) {

      toggleLights();

      Serial.println("Dark environment: Lights toggled");

    }
    else {

      Serial.println("Environment is bright: Lights remain unchanged");

    }
  }


  // Button pressed
  if (buttonFlag) {

    buttonFlag = false;

    toggleLights();

    Serial.println("Button pressed: Lights toggled");

    // Small debounce delay
    delay(200);
  }
}