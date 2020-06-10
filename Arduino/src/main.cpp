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
const int openCurtainSwitchPin = 35;
const int closeCurtainSwitchPin = 34;

const char *ssid = "koffiekoekjes";
const char *password = "herriehuis";

const float curtainspeed = 10.0f;

AsyncWebServer server(80);
Tb6612fng motor(STBY, AIN1, AIN2, PWMA);

enum States
{
  Open,
  Close,
  Stop
};

States currentState = States::Stop;


void openCurtain(DynamicJsonDocument &json)
{
  currentState = States::Open;
}

void stopCurtain(DynamicJsonDocument &json)
{
  currentState = States::Stop;
}

void closeCurtain(DynamicJsonDocument &json)
{
  currentState = States::Close;
}

/*
String getMotorPosition(DynamicJsonDocument& json) {
  long currentPosition = stepper.currentPosition();

  StaticJsonDocument<51> outJson;
  outJson["position"] = 0;

  String outString;

  serializeJson(outJson, outString);

  return outString;
}
*/

void setup()
{
  motor.begin();

  pinMode(ledPin, OUTPUT);
  pinMode(openCurtainSwitchPin, INPUT);  
  pinMode(closeCurtainSwitchPin, INPUT);

  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on(
      "/",
      HTTP_GET,
      [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", "<h1>Curino<h1>");
      });

  server.on(
      "/post",
      HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        DynamicJsonDocument doc(len);

        deserializeJson(doc, data);
        String command = doc["command"];

        if (command == "openCurtain")
        {
          openCurtain(doc);
        }
        else if (command == "closeCurtain")
        {
          closeCurtain(doc);
        }
        else if (command == "stopCurtain")
        {
          stopCurtain(doc);
        }

        Serial.println(command);
        request->send(200);
      });

  server.begin();
}

void loop()
{
  switch (currentState)
  {
  case States::Open: // Opening curtain.
    motor.drive(curtainspeed);
    Serial.println(digitalRead(openCurtainSwitchPin));
    if (digitalRead(openCurtainSwitchPin) == 1)
      currentState = States::Stop;
    break;

  case States::Close: // Closing curtain.
    motor.drive(-curtainspeed);
    if (digitalRead(closeCurtainSwitchPin) == 1)
      currentState = States::Stop;
    break;

  case States::Stop: // Stop the motor.
    motor.brake();//als code niet werkt dan mag justin niet boos worden
    break;
  }
}