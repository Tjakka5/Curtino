#include <Arduino.h>
#include <WiFi.h>
#include "HTTPSRedirect.h"

const char *ssid = "";
const char *password = "";

// begin remote
WiFiClient client;

char server[] = "84.82.92.7";
// eind remote
// begin opdracht c
const char* host = "script.google.com";
const char* GScriptId = "AKfycbxrb3ZCGKvLJLzyEPDBZVqEgH8xYl7TU77BJ2zSteHzfRSUUTcs";

const int httpsPort = 443;
HTTPSRedirect* httpsclient = nullptr;

String url = String("/macros/s/") + GScriptId + "/exec?";
const char* fingerprint = "10 76 E5 EC 2C ED 13 E1 CD 00 25 52 B3 A4 9C C1 83 CF 53 67";

int intervaltime = 1000 * 60 * 10; // 1000 ms * 1 minute * 10 minutes
int nextmillis;
// eind opdracht c

void doOpenRequest() {
  char json[] = "{\"command\": \"openCurtain\", \"deviceID\": \"Curtino\"}";

  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("POST /sendCommand HTTP/1.1");
    client.println("Host: 84.82.92.7");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Content-Type: application/json");
    client.print("Content-Length: "); client.println(strlen(json)); // lengte open 54 lengte dicht 55
    client.println();
    client.println(json);
    client.println();
  }
}

void doCloseRequest() {
  char json[] = "{\"command\": \"closeCurtain\", \"deviceID\": \"BigPeePeeESP\"}";

  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("POST /sendCommand HTTP/1.1");
    client.println("Host: 84.82.92.7");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Content-Type: application/json");
    client.print("Content-Length: "); client.println(strlen(json)); // lengte open 54 lengte dicht 55
    client.println();
    client.println(json);
    client.println();
  }
}

// esp32 workaround
void initHttpsClient() {
  httpsclient = new HTTPSRedirect(httpsPort);
  httpsclient->setPrintResponseBody(false);
  httpsclient->setContentTypeHeader("application/json");

  Serial.println("Initializing httpsclient");

  // Try to connect for a maximum of 5 times
  bool flag = false;
  for (int i=0; i<5; i++){
    int retval = httpsclient->connect(host, httpsPort);
    if (retval == 1) {
       flag = true;
       break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }

  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    return;
  }
}

void doTempToSheets() {
  // constanten
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  int value = analogRead(32);
  float tempstep = log(10000 * (4095.0 / (float)value - 1.0));
  float temp = (1.0 / (c1 + c2* tempstep + c3 * tempstep * tempstep * tempstep)) - 273.15;

  // omdat de esp32 blijkbaar geen setInsecure functie heeft verwijderen we elke keer de httpsclient library
  // als we dit niet doen kan een error het hele systeem vastzetten
  // ook hebben we wat aanpassingen moeten maken in de HTTPSRedirect (en zelfs de Wifisecure) library om dit te faciliteren
  initHttpsClient();
  Serial.println(temp);

  httpsclient->GET(url + "value=" + String(temp) + "&tag=Temperatuur" + "&sheet=Groep06", host);
  delete httpsclient;
}

void setup() {
  pinMode(32, INPUT); // opdracht c temperatuur
  pinMode(34, INPUT); // knop 1 remote
  pinMode(35, INPUT); // knop 2 remote

  WiFi.begin(ssid, password);
  Serial.begin(9600);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }
  initHttpsClient();
  nextmillis = millis();
}

void loop() {
  if (digitalRead(35) == HIGH) {
    doOpenRequest();
  }
  if (digitalRead(34) == HIGH) {
    doCloseRequest();
  }

  if (millis() > nextmillis) {
    doTempToSheets();

    nextmillis = millis() + intervaltime;
  }
}

