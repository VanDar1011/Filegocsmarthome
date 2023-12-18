//handle function: send webpage to client
//=======================================
void webpage()
{
  server.send(200, "text/html", webpageCode);
}
//=====================================================
//function process event: new data received from client
//=====================================================
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
      }
      break;

    case WStype_TEXT:
      String payloadString = (const char *)payload;
      Serial.print("Received payloadString: ");
      Serial.println(payloadString);

      // Parse the received JSON string into a JSON object
      DynamicJsonDocument doc(256);
      deserializeJson(doc, payloadString);

      // Access the values in the JSON object
      const char *action = doc["action"];

      if (strcmp(action, "toggleLed1") == 0) {
        led1State = !led1State;
        Serial.print("Led1:");
        Serial.println(led1State);
        if (led1State == 1) {
          digitalWrite(LED1_PIN, HIGH);
          Serial.println("Led1 sang");
        }  else {
          digitalWrite(LED1_PIN, LOW);
          Serial.println("Led1 tat");
        }

      }
      if (strcmp(action, "toggleLed2") == 0) {
        led2State = !led2State;
        Serial.print("Led2:");
        Serial.println(led2State);
        if (led2State == 1) {
          digitalWrite(LED2_PIN, HIGH);
          Serial.println("Led2 sang");
        }  else {
          digitalWrite(LED2_PIN, LOW);
          Serial.println("Led2 tat");
        }
      }
        break;
      }
  }
