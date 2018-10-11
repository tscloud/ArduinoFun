/*
  GUS_base.h - Grand Unified Sensor - Base.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "FS.h"
#include "ArduinoJson.h"
#include "WiFiClient.h"
#include "PubSubClient.h"

#define CONFIG_FILE  "/config.json"

class GUS_base {
  public:
    GUS_base();
    //~GUS_base();
    char mqtt_channel[50];
    PubSubClient client;
    void setup();
    void loop();
  private:
    // used for wifi
    char ssid[10];
    char password[20];
    char myhostname[10];
    // used for MQTT
    char mqtt_server[10];
    int  mqtt_port = 0;
    char mqtt_clientid[20];
    WiFiClient espClient;
    unsigned long delayTime = 1000;
    void readConfig();
    void wifisetup();
    void reconnect();
    File GetFile(String fileName);
};