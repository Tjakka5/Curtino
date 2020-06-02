#include <Arduino.h>

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"

const int ledPin = 2;
int ledState = 0;

const char* ssid = "koffiekoekjes";
const char* password =  "herriehuis";
 
AsyncWebServer server(80);

void updateLedState() {
  digitalWrite(ledPin, ledState);
}

void setLed(DynamicJsonDocument& json) {
  boolean _state = json["state"];
  int state = 0;
  
  if (json.containsKey("state")) {
    Serial.println("Yes");
  } else {
    Serial.println("NO >:(");
  }

  Serial.println(_state);

  if (_state == true) {
    state = 1;
  } else if (_state == false) {
    state = 0;
  }

  ledState = state;

  updateLedState();
}

void getLed(DynamicJsonDocument json) {

}

void toggleLed(DynamicJsonDocument json) {

}

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
  
  server.on(
    "/",
    HTTP_GET,
    [](AsyncWebServerRequest * request) {
      Serial.println("Hello!");

      request->send(200);
    }
  );

  server.on(
    "/post",
    HTTP_POST,
    [](AsyncWebServerRequest * request) { },
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
      DynamicJsonDocument doc(len);

      deserializeJson(doc, data);
      String command = doc["command"];

      if (command == "setLed") {
        Serial.println("Executing setLed");
        setLed(doc);
      }

      Serial.println(command);

      request->send(200);
  });
 
  server.begin();
}

void loop() {}

/*
#include <WiFi.h>

const char *ssid = "koffiekoekjes";
const char *passphrase = "herriehuis";

WiFiServer webServer(80);
String header;

void setup()
{
  Serial.begin(9600);

  WiFi.begin(ssid, passphrase);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
  }

  Serial.println("Connected to WiFi!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  webServer.begin();
}

void loop()
{
  WiFiClient client = webServer.available();

  if (client)
  {
    Serial.println("New client connected");

    String currentLine = "";

    while (client.connected())
    {

      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            client.println("<body><h1>ESP32 Web Server</h1>");

            client.println("</body></html>");

            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }

    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
*/