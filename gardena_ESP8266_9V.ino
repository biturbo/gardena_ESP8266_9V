#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <time.h>
#include <FS.h>
#include <Wire.h>

void do_setup();
void do_loop();

void setup() {
  do_setup();
}

void loop() {
  do_loop();
}
