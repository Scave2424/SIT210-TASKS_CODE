// Task 1.1P Switching ON Lights (Modular Programming Approach)

const int buttonPin = 2;
const int porchLightPin = 3;
const int hallwayLightPin = 4;

const unsigned long porchDuration = 30000;   // 30 seconds
const unsigned long hallwayDuration = 60000; // 60 seconds

bool porchLightOn = false;
bool hallwayLightOn = false;

unsigned long porchStartTime = 0;
unsigned long hallwayStartTime = 0;

int lastButtonState = LOW;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(porchLightPin, OUTPUT);
  pinMode(hallwayLightPin, OUTPUT);
}

void loop() {
  checkButtonPress();
  updatePorchLight();
  updateHallwayLight();
}

// Watches the button. On a fresh press (LOW - HIGH) turns both lights on.
void checkButtonPress() {
  int currentButtonState = digitalRead(buttonPin);

  if (currentButtonState == HIGH && lastButtonState == LOW) {
    turnOnPorchLight();
    turnOnHallwayLight();
  }

  lastButtonState = currentButtonState;
}

// Switches the porch light on and marks the time it happened.
void turnOnPorchLight() {
  digitalWrite(porchLightPin, HIGH);
  porchLightOn = true;
  porchStartTime = millis();
}

// Switches the hallway light on and marks the time it happened.
void turnOnHallwayLight() {
  digitalWrite(hallwayLightPin, HIGH);
  hallwayLightOn = true;
  hallwayStartTime = millis();
}

// Checks whether the porch light's 30 seconds are up and if so turns it off.
void updatePorchLight() {
  if (porchLightOn && (millis() - porchStartTime >= porchDuration)) {
    digitalWrite(porchLightPin, LOW);
    porchLightOn = false;
  }
}

// Checks whether the hallway light's 60 seconds are up and if so turns it off.
void updateHallwayLight() {
  if (hallwayLightOn && (millis() - hallwayStartTime >= hallwayDuration)) {
    digitalWrite(hallwayLightPin, LOW);
    hallwayLightOn = false;
  }
}
