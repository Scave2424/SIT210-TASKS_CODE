#include <WiFiNINA.h>

// Wi-Fi credentials used by both the Arduino and laptop.
char ssid[] = "MKS A201";
char pass[] = "12344321";

// Create the Arduino HTTP server on port 80.
WiFiServer server(80);

// Assign one pin to each room LED.
const int livingRoomLED = 2;
const int bathroomLED = 3;
const int closetLED = 4;

// Store the current ON/OFF state of each LED.
bool livingRoomState = false;
bool bathroomState = false;
bool closetState = false;


// Toggle the LED that matches the room name received.
void toggleRoom(String room) {

  if (room == "living room") {
    livingRoomState = !livingRoomState;
    digitalWrite(livingRoomLED, livingRoomState);
  }

  else if (room == "bathroom") {
    bathroomState = !bathroomState;
    digitalWrite(bathroomLED, bathroomState);
  }

  else if (room == "closet") {
    closetState = !closetState;
    digitalWrite(closetLED, closetState);
  }
}


void setup() {

  Serial.begin(9600);

  // Configure the three LED pins as outputs.
  pinMode(livingRoomLED, OUTPUT);
  pinMode(bathroomLED, OUTPUT);
  pinMode(closetLED, OUTPUT);

  // Start the system with all three LEDs switched OFF.
  digitalWrite(livingRoomLED, LOW);
  digitalWrite(bathroomLED, LOW);
  digitalWrite(closetLED, LOW);

  // Keep trying until the Arduino connects to the Wi-Fi network.
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Connecting to Wi-Fi...");
    delay(3000);
  }

  // Start listening for HTTP requests from server.js.
  server.begin();

  // Display the Arduino IP address needed in server.js.
  Serial.print("Arduino IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

  // Check whether server.js has connected to the Arduino.
  WiFiClient client = server.available();

  if (client) {

    // Read the HTTP request sent by server.js.
    String request = client.readStringUntil('\n');

    Serial.println(request);

    // Identify the requested room and call toggleRoom() with the correct name.
    if (request.indexOf("GET /toggle?room=livingroom") >= 0) {
      toggleRoom("living room");
    }

    else if (request.indexOf("GET /toggle?room=bathroom") >= 0) {
      toggleRoom("bathroom");
    }

    else if (request.indexOf("GET /toggle?room=closet") >= 0) {
      toggleRoom("closet");
    }

    // Send confirmation back to server.js after processing the request.
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("LED command completed");

    // Close the HTTP connection after the response is sent.
    client.stop();
  }
}