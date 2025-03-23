// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL3pkCAQUrt"
#define BLYNK_TEMPLATE_NAME "Room Auto"
#define BLYNK_AUTH_TOKEN "wCNRCJNTFQFhlxbRupY7x2J-cLuIfHum"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials
const char* ssid = "shubham";      // Replace with your WiFi network name
const char* password = "12345678";  // Replace with your WiFi password

// Define the GPIO pins for the switches
const int SWITCH_1_PIN = 23;  // Digital pin connected to switch 1
const int SWITCH_2_PIN = 22;  // Digital pin connected to switch 2
const int SWITCH_3_PIN = 21;  // Digital pin connected to switch 3
const int SWITCH_4_PIN = 19;  // Digital pin connected to switch 4

// Switch status
bool switch1State = false;
bool switch2State = false;
bool switch3State = false;
bool switch4State = false;

// Initialize timer
BlynkTimer timer;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize the GPIO pins as outputs
  pinMode(SWITCH_1_PIN, OUTPUT);
  pinMode(SWITCH_2_PIN, OUTPUT);
  pinMode(SWITCH_3_PIN, OUTPUT);
  pinMode(SWITCH_4_PIN, OUTPUT);
  
  // Set initial state of pins to HIGH (relays off for active-low relays)
  digitalWrite(SWITCH_1_PIN, HIGH);
  digitalWrite(SWITCH_2_PIN, HIGH);
  digitalWrite(SWITCH_3_PIN, HIGH);
  digitalWrite(SWITCH_4_PIN, HIGH);
  
  // Connect to WiFi and Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  
  Serial.println("Connected to Blynk!");
  
  // Force all switch states to be OFF initially on the Blynk server
  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V3, 0);
  
  // Initialize Blynk Timer
  timer.setInterval(30000L, checkConnection);  // Check connection every 30 seconds
  timer.setInterval(5000L, updateStateSync);   // Sync states every 5 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}

// Function to sync states more frequently
void updateStateSync() {
  updateBlynkStatus();
}

// Function to check connection status
void checkConnection() {
  if (Blynk.connected()) {
    Serial.println("Blynk connection OK");
  } else {
    Serial.println("Blynk connection lost");
    // Attempt to reconnect
    Blynk.connect();
  }
}

// Function to update Blynk with current switch statuses
void updateBlynkStatus() {
  // Update virtual pins with switch statuses
  Blynk.virtualWrite(V0, switch1State ? 1 : 0);
  Blynk.virtualWrite(V1, switch2State ? 1 : 0);
  Blynk.virtualWrite(V2, switch3State ? 1 : 0);
  Blynk.virtualWrite(V3, switch4State ? 1 : 0);
}

// This function is called when Switch 1 is toggled in the app
BLYNK_WRITE(V0) {
  int pinValue = param.asInt();
  switch1State = (pinValue == 1);
  
  // Update physical switch state (LOW to turn ON for active-low relays)
  digitalWrite(SWITCH_1_PIN, switch1State ? LOW : HIGH);
  Serial.print("Switch 1 set to: ");
  Serial.println(switch1State ? "ON" : "OFF");
}

// This function is called when Switch 2 is toggled in the app
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  switch2State = (pinValue == 1);
  
  // Update physical switch state (LOW to turn ON for active-low relays)
  digitalWrite(SWITCH_2_PIN, switch2State ? LOW : HIGH);
  Serial.print("Switch 2 set to: ");
  Serial.println(switch2State ? "ON" : "OFF");
}

// This function is called when Switch 3 is toggled in the app
BLYNK_WRITE(V2) {
  int pinValue = param.asInt();
  switch3State = (pinValue == 1);
  
  // Update physical switch state (LOW to turn ON for active-low relays)
  digitalWrite(SWITCH_3_PIN, switch3State ? LOW : HIGH);
  Serial.print("Switch 3 set to: ");
  Serial.println(switch3State ? "ON" : "OFF");
}

// This function is called when Switch 4 is toggled in the app
BLYNK_WRITE(V3) {
  int pinValue = param.asInt();
  switch4State = (pinValue == 1);
  
  // Update physical switch state (LOW to turn ON for active-low relays)
  digitalWrite(SWITCH_4_PIN, switch4State ? LOW : HIGH);
  Serial.print("Switch 4 set to: ");
  Serial.println(switch4State ? "ON" : "OFF");
}

// This function will be called when the ESP32 connects to Blynk server
BLYNK_CONNECTED() {
  // Request the latest states from the server
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  
  // After syncing, update Blynk with current status
  updateBlynkStatus();
}