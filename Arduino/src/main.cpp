#include <Arduino.h>

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include <TB6612FNG.h>

#define AIN1 26
#define AIN2 25
#define PWMA 33
#define STBY 27

const int ledPin = 2;

const char* ssid = "koffiekoekjes";
const char* password =  "herriehuis";
 
AsyncWebServer server(80);
Tb6612fng motor(STBY, AIN1, AIN2, PWMA);

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
}

String getMotorPosition(DynamicJsonDocument& json) {
  /*
  long currentPosition = stepper.currentPosition();
  */

  StaticJsonDocument<51> outJson;
  outJson["position"] = 0;

  String outString;

  serializeJson(outJson, outString);

  return outString;
}

void setup() {
  motor.begin();

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

  // 500ms forwards
  motor.drive(0.5, 500);
  // 500ms backwards
  motor.drive(-0.5, 500);
  // Full range of motor speed
  for (auto i = 1; i <= 10; i += 1) {
    motor.drive(0.1 * i, 200, false);
  }
  motor.brake();
  delay(1000);
}