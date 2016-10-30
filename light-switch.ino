/*
 * Light Switch sketch
 *
 * Make sure to add SSID, PASSWORD, and LIFX_BEARER_TOKEN_HEADER to your config.h file
 */

#include <ESP8266WiFi.h>
#include "RestClient.h"

#include "config.h"

#define SWITCH 3

RestClient restClient = RestClient("api.lifx.com", 443, "E3 69 05 13 32 74 C0 37 F8 6C B8 A7 18 98 87 B7 CD DD 86 F0");

void setup() {
  WiFiClientSecure client;

  Serial.begin(9600);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  setLights(false);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWITCH, INPUT);
}

void setLights (bool state) {
  String power;
  if (state) {
    power = "on";
  } else {
    power = "off";
  }

  String response = "";
  restClient.setHeader(LIFX_BEARER_TOKEN_HEADER);
  int statusCode = restClient.put("/v1/lights/all/state", ("{\"power\": \"" + power + "\"}").c_str(), &response);

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.println("Response:");
  Serial.println(response);
}

void toggleLights (bool state) {
  String power;
  if (state) {
    power = "on";
  } else {
    power = "off";
  }

  String response = "";
  restClient.setHeader(LIFX_BEARER_TOKEN_HEADER);
  int statusCode = restClient.put("/v1/lights/all/toggle", "", &response);

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.println("Response:");
  Serial.println(response);
}

bool switchState = true;
bool oldSwitchState = false;

void loop() {
  switchState = digitalRead(SWITCH) == HIGH;

  Serial.println(switchState);

  digitalWrite(LED_BUILTIN, digitalRead(SWITCH));

  if (switchState != oldSwitchState) {
    Serial.println(switchState ? "Switch on" : "Switch off");
    setLights(switchState);
  }

  delay(100);

  oldSwitchState = switchState;
}
