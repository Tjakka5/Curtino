#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "ZyXELC532C1";
const char *password = "nyjo1815";

WiFiClient client;

char server[] = "84.82.92.7";

void doOpenRequest() {
  char json[] = "{\"command\": \"openCurtain\", \"deviceID\": \"bigPeePeeESP\"}";

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
  char json[] = "{\"command\": \"CloseCurtain\", \"deviceID\": \"bigPeePeeESP\"}";

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

void setup() {
  WiFi.begin(ssid, password);
  Serial.begin(9600);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }
  
  while (true) {
    doOpenRequest();
    delay(10000);
    doCloseRequest();
    delay(10000);
  }
}

void loop() {
  
}

