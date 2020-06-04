#include <Arduino.h>

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"

#include <AccelStepper.h>

const int ledPin = 2;

const char* ssid = "koffiekoekjes";
const char* password =  "herriehuis";
 
AsyncWebServer server(80);
AccelStepper stepper(AccelStepper::FULL4WIRE, 26, 25, 33, 32);


void setLed(DynamicJsonDocument& json) {
  if (!json.containsKey("state")) {
    Serial.println("owo");
    // TODO: Return fault code
  }

  boolean _state = json["state"];
  digitalWrite(ledPin, _state);
}

void setMotorPosition(DynamicJsonDocument& json) {
  if (!json.containsKey("position")) {
    // TODO: Return fault code
  }

  int _position = json["position"];
  
  stepper.moveTo(_position);
}

String getMotorPosition(DynamicJsonDocument& json) {
  long currentPosition = stepper.currentPosition();

  StaticJsonDocument<51> outJson;
  outJson["position"] = currentPosition;

  String outString;

  serializeJson(outJson, outString);

  return outString;
}

void setup() {
  pinMode(ledPin, OUTPUT);

  stepper.setMaxSpeed(100);
  stepper.setAcceleration(100);

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
      request->send(200, "text/html", "<h1>Curino<h1>");
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
        setLed(doc);
      } else if (command == "setMotorPosition") {
        setMotorPosition(doc);
      } else if (command == "getMotorPosition") {
        String out = getMotorPosition(doc);
        request->send(200, "application/json", out);
      }

      Serial.println(command);
      request->send(200);
  });
 
  server.begin();
}

void loop() {
  stepper.run();
}