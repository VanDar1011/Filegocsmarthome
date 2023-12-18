#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h> 
#include <ArduinoJson.h>
// khai báo các thư viện
//-----------------------------------------------
const char* ssid = "P.3819";
const char* password = "meomeo..3"; // khai bao wifi
//-----------------------------------------------
bool led1State = false;
bool led2State = false;
#define LED1_PIN 12
#define LED2_PIN 13
 //-----------------------------------------------
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
//-----------------------------------------------

//-----------------------------------------------
#include "html_page.h"
#include "functions.h"
//====================================================================
void setup()
{
  Serial.begin(115200); 
  //-----------------------------------------------
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  WiFi.mode(WIFI_STA);
  pinMode(LED1_PIN,OUTPUT);
   pinMode(LED2_PIN,OUTPUT);
  Serial.println();
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  //-----------------------------------------------
  server.on("/", webpage);
  //-----------------------------------------------
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}
//====================================================================
void loop(){
  webSocket.loop();
  server.handleClient(); //xu li http request gui den

 DynamicJsonDocument doc(256);
  doc["led1"] = led1State ? "ON" : "OFF";
  doc["led2"] = led2State ? "ON" : "OFF";

  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  // Send the JSON string to all connected clients
  webSocket.broadcastTXT(jsonString);
}
