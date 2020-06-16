#include <Arduino.h>

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include <TB6612FNG.h>
#include <NTPClient.h>

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

const int ldrpin = 32;
float lightValue = 0.0f;

AsyncWebServer server(80);
Tb6612fng motor(STBY, AIN1, AIN2, PWMA);
WiFiClient client;

char serverIP[] = "192.168.2.19";
char json[] = "{\"deviceID\": \"BigPeePeeESP\", \"deviceIP\": \"192.168.2.11\"}";

enum States
{
  Idle,
  Opening,
  Closing
};

enum CheckingType
{
  None,
  Time,
  Light
};

/*
enum LightStates
{
  Open,
  Close
};
*/

States currentState = States::Idle;
CheckingType checkingType = CheckingType::None;

int timeToOpenFromMidnight = 0;
int timeToCloseFromMidnight = 0;

int lightRequiredToOpen = 0;
int lightRequiredToClose = 0;

//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void openCurtain()
{
  currentState = States::Opening;
}

void commandOpenCurtain(DynamicJsonDocument &json)
{
  openCurtain();
}


void stopCurtain()
{
  currentState = States::Idle;
}

void commandStopCurtain(DynamicJsonDocument &json)
{
  stopCurtain();
}


void closeCurtain()
{
  currentState = States::Closing;
}

void commandCloseCurtain(DynamicJsonDocument &json)
{
  closeCurtain();
}


void configureTime(DynamicJsonDocument &json)
{
  int openTime = atoi(json["openTime"]);
  int closeTime = atoi(json["closeTime"]);
}

void configureLight(DynamicJsonDocument &json)
{
  //lightRequiredToClose = json["lightRequiredToClose"];
  //lightRequiredToOpen = json["lightRequiredToOpen"];
  lightRequiredToClose = 40;
  lightRequiredToOpen = 70;

  checkingType = CheckingType::Light;
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
  pinMode(ldrpin, INPUT);

  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }

  
  Serial.println(WiFi.localIP());

  if (client.connect(serverIP, 80)) {
    client.println("POST /registerDevice HTTP/1.1");
    client.println("Content-Type: application/json");
    client.print("Content-Length: "); client.println(strlen(json));
    client.println();
    client.println(json);
    client.println();
  } 

  //timeClient.begin();


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

        Serial.println("Command received");

        deserializeJson(doc, data);
        String command = doc["command"];

        Serial.println(command);

        if (command == "openCurtain")
        {
          commandOpenCurtain(doc);
        }
        else if (command == "closeCurtain")
        {
          commandCloseCurtain(doc);
        }
        else if (command == "stopCurtain")
        {
          commandStopCurtain(doc);
        }
        else if (command == "configureTime")
        {
          configureTime(doc);
        }
        else if (command == "configureLight")
        {
          configureLight(doc);
        }

        
        request->send(200);
      });

  server.begin();
}

bool isOpen() {
  return (digitalRead(openCurtainSwitchPin) == 1);
}

bool isClosed() {
  return (digitalRead(closeCurtainSwitchPin) == 1);
}



void loop()
{
  //timeClient.update();

  int _lightValue = analogRead(ldrpin);
  int lightValue = map(_lightValue, 0, 4095, 0, 100);

  if (checkingType == CheckingType::Light) {
    if (currentState != States::Opening && !isOpen() && lightValue >= lightRequiredToOpen)
    {
      Serial.println("opening");
      openCurtain();
    }
    else if (currentState != States::Closing && !isClosed() && lightValue <= lightRequiredToClose)
    {
      Serial.println("closing");
      closeCurtain();
    }
  }

  switch (currentState)
  {
  case States::Opening: // Opening curtain.
    motor.drive(curtainspeed);
    if (isOpen())
      currentState = States::Idle;
    break;

  case States::Closing: // Closing curtain.
    motor.drive(-curtainspeed);
    if (isClosed())
      currentState = States::Idle;
    break;

  case States::Idle: // Stop the motor.
    motor.brake();
    break;
  }
}