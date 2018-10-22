/*
  GUS_base.cpp - Grand Unified Sensor - Base.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_base.h"

GUS_base::GUS_base() {
  /*
  client = PubSubClient(espClient);

  readConfig();
  wifisetup();

  // setup MQTT
  client.setServer(mqtt_server, mqtt_port);
  */
}

void GUS_base::setup() {
  client = PubSubClient(espClient);

  readConfig();
  wifisetup();

  // setup MQTT
  client.setServer(mqtt_server, mqtt_port);
}

void GUS_base::loop() {
  // MQTT connect if we're not connected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // delay delay time read from config
  delay(delaytime);
}

void GUS_base::pubData(char *sname, float temp, float humidity, float pressure) {
  // publish real data

  // (re)init array & channel name
  result[0] = {'\0'};
  locchannel[0] = {'\0'};

  // use Arduino String to build dat to be published
  String sResult = "";
  sResult = buildResult(temp, "T", sResult);
  sResult = buildResult(humidity, "H", sResult);
  sResult = buildResult(pressure, "P", sResult);
  sResult.toCharArray(result, sizeof(result));

  // DEBUG
  Serial.print("result: ");
  Serial.println(sResult);

  // append sensor name to MQTT channel
  strcat(locchannel, mqtt_channel);
  strcat(locchannel, "/");
  strcat(locchannel, sname);

  // DEBUG
  Serial.print("channel: ");
  Serial.println(locchannel);

  client.publish(locchannel, result);
}

String GUS_base::buildResult(float lval, String ltype, String lresult) {
  // potentially concat value & type to potentially previously built result
  if (lval != 0) {
    if (lresult.length() != 0) {
      lresult += ",";
    }
    lresult += ltype + lval;
  }

  return lresult;
}

void GUS_base::wifisetup() {
  // set hostname
  WiFi.hostname(myhostname);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print(F("\n\r \n\rWorking to connect"));

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(F("GUS lib output"));
  Serial.print(F("Connected to "));
  Serial.println(ssid);
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("hostname: "));
  Serial.println(WiFi.hostname());
}

void GUS_base::reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_clientid)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("test_channel","hello world");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// file io
File GUS_base::GetFile(String fileName) {
  File textFile;
  if (SPIFFS.exists(fileName)) {
    textFile = SPIFFS.open(fileName, "r");
  }
  return textFile;
}

void GUS_base::readConfig() {
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");

    // parse json config file
    File jsonFile = GetFile(CONFIG_FILE);
    if (jsonFile) {
      // Allocate a buffer to store contents of the file.
      size_t size = jsonFile.size();
      std::unique_ptr<char[]> jsonBuf(new char[size]);
      jsonFile.readBytes(jsonBuf.get(), size);

      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(jsonBuf.get());
      if (json.success()) {
        strcpy(ssid, json["ssid"]);
        strcpy(password, json["password"]);
        delaytime = json["delaytime"];
        strcpy(myhostname, json["myhostname"]);
        strcpy(mqtt_server, json["mqtt_server"]);
        mqtt_port = json["mqtt_port"];
        strcpy(mqtt_clientid, json["mqtt_clientid"]);
        strcpy(mqtt_channel, json["mqtt_channel"]);
      } else {
        Serial.println("failed to load json config");
      }
      jsonFile.close();
    }
    SPIFFS.end();
  }
}
