/* 
 * httpRC webserver firmware for ESP8266 (ESP-01)for PiKoder/SSC wRX 
 *
 * The ESP8266-01 SoC Board offers an access point (ap) with a web page to any 
 * browser supporting XMLHttpRequest() and relays the respective commands to the 
 * PiKoder/SSC's UART.
 * 
 * The default settings for the ap are: SSID: "PiKoder_wRX", Password: "password". 
 * 
 * This sketch is based on the "esp_microe-buggy" povided by ChedaDSP@Makers-NS. 
 * Please refer to https://www.hackster.io/16283/esp8266-mikroe-buggy-ccd102 for 
 * more information.
 */ 

/* 
 * Copyright 2017 Gregor Schlechtriem
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * This sketch requires Arduino IDE (Version > 1.6.8). The following settings are recommended:
 *
 * Add ESP board: 
 * File -> Preferences -> Additional Boards Manager URLs:
 * enter "http://arduino.esp8266.com/staging/package_esp8266com_index.json" 
 * and then using:
 * Tools -> Boards -> Boards Manager to install the esp8266
 *
 * Recommended board settings:
 * Board: "Generic ESP8266 module"
 * Flash Mode: "QIO"
 * Flash Frequency: "40MHz"
 * Upload Using: "Serial"
 * CPU Frequency: "80MHz"
 * Flash Size: "1M (64K SPIFFS)"
 * Upload Speed "115200"
 * Port: "COM40" (depends on your setup)
 * Programmer: "Arduino Gemma"
 *
 * Please refer to www.makerprojects.de and www.pikoder.de for more information.
 */ 

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <Hash.h>
#include "buggy.h"

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiSSID[] = "PiKoder_wRX";
const char WiFiAPPSK[] = "password";
const char host[] = "buggy";

ESP8266WebServer server(80);

void drive(){
  int direction = 0, throttle = 0; 
  if(server.hasArg("d")) {
    direction = server.arg("d").toInt();
  }
    if(server.hasArg("t")) {
    throttle = server.arg("t").toInt();
  }
  
  moveBuggy(direction, throttle);
  
  //Must play nice and return 200OK
  server.send(200, "text/plain", server.arg(0));
}

void setup(void)
{
  setupBuggy();

  Serial.begin(9600);

  // Turn on AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WiFiSSID, WiFiAPPSK);
  
  server.begin();
  
  SPIFFS.begin();
  MDNS.begin(host);

  //SERVER INIT
  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  // REST end point
  server.on("/drive", drive );
  
  server.begin();
  // Add service to MDNS
  MDNS.addService("http", "tcp", 80);

}
 
void loop(void)
{
  server.handleClient();
}
