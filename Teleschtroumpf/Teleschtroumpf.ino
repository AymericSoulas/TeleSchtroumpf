#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"
#include "Convert.h"
#include "Server_callbacks.h"
#include "Tasks.h"

// Here we define the pin used for the command of the led
#define LED 36

// Here we define the timers for the waiting
#define DOT_TIMER 500
#define DASH_TIMER 1000
#define INTER_DOT_DASH_TIMER 500
#define LETTER_TIMER 1000
#define WORD_TIMER 2000
#define PHRASE_TIMER 3000

// The SSID and Password are in a separate .h file (not commited) under the following presentation
// #define SSID "The SSID"
// #define PASSWORD "The Password"

// We are using the ESP-32 as an Access Point so we have to define the characteristics of this AP
IPAddress local_ip(192, 168, 4, 1);   // Fixed IP
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

//Starting Web Server
WebServer server(80);

// Global Variables to stock the messages
String current_message = "";
String buffer_message = "";
String morse_message = ";";
volatile bool new_message = 0;
int diffusion_count = 0;

// Definition of the mutex message used to prevent trouble with variable access from different threads
SemaphoreHandle_t mutex_message;

// We stop the system until the setup is completely finished
volatile bool system_ready = false;

// Setup
void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }  // Wait for the port to be open
  delay(500);
  Serial.println("Test");

  // Setting up the WiFi in AP Mode
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  // Setting Up Multi-Tasking
  // To prevent conflicts on the Message that is used by the WiFi task and the Morse one we need to make a MuTex
  mutex_message = xSemaphoreCreateMutex();

  // Setting Up The WiFi-Functions
  xTaskCreatePinnedToCore(
    tacheWiFi,    // Function
    "WiFi",       // Name (debug)
    8192,         // Stack Size
    NULL,         // Parameters
    1,            // Priority (0 = LOW, 25 = HIGH)
    NULL,         // handle
    0             // core 0
  );

  // Verification
  Serial.print("Access Point Started : ");
  Serial.println(AP_SSID);
  Serial.print("IP : ");
  Serial.println(WiFi.softAPIP());  // Must print 192.168.4.1

  // Routes
  server.on("/", handleRoot);
  server.on("/morse", HTTP_POST, handleMorse);
  server.begin();
  Serial.println("Server Started.");

  // Pin Setup
  pinMode(LED, OUTPUT);

  //We can start the loop
  system_ready = true;
}

void loop() {
  if (!system_ready) return;
  // The morse translation's and the light's code run in the main loop running on Core 1 While the WiFi runs in the Core 0.
  // If we receive a new message, we interrupt the diffusion of the previous one and we translate the new one.
  if(new_message){
    Serial.println("Test1");
    if (xSemaphoreTake(mutex_message, portMAX_DELAY)){
      buffer_message = current_message;
      xSemaphoreGive(mutex_message);
      new_message = 0;
    }
    Serial.println("Test2");
    morse_message = convertPhraseToMorse(buffer_message);
    Serial.println(morse_message);

    // After translating the message, we reset everything.
    diffusion_count = 0;
    digitalWrite(LED, LOW);
  }else{
    // In the case in which we are at the first part of a phrase, we wait for 3 seconds to mark the start of the phrase.
    if (diffusion_count == 0){
      digitalWrite(LED, LOW);

    }
  }

  

}
