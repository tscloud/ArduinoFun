/*
  GUS_base.h - Grand Unified Sensor - Base.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/

#ifndef GUS_BASE_H
#define GUS_BASE_H

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
    unsigned long delaytime = 1000;
    void setup();
    void loop();
    void pubData(char *sname, float temp, float humidity, float pressure);
  private:
    // used for wifi
    char ssid[10];
    char password[20];
    char myhostname[10];
    // used for MQTT
    char mqtt_server[10];
    int  mqtt_port = 0;
    char mqtt_clientid[20];
    char mqtt_channel[50];
    // this will be passed as the published data
    // NOTE on data structure: "-" sign not currently accounted for
    //  Also need space for sensor name - MAX 10
    char result[30]; // T[+/-]xx.x,Hyy.y,Pzz.z <-- greatest length: 20
    // used to build published data - char[]
    char locchannel [70]; // big enough to hold mqtt_channel + sensor name
    WiFiClient espClient;
    PubSubClient client;
    void readConfig();
    void wifisetup();
    void reconnect();
    File GetFile(String fileName);
    String buildResult(float val, String type, String result);
};
#endif