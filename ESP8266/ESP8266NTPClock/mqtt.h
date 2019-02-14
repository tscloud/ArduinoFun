/*
 *  mqtt.h - mqtt stuff to do
 *
 *  Concept, Design and Implementation by: Tom Cloud
 *  Last Update: Feb 09, 2019
 */

#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <PubSubClientTools.h>

// only need this for publishing?
//#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
//#include <ThreadController.h>

#define MQTT_SERVER "bigasspi.fios-router.home"

WiFiClient espClient;
PubSubClient client(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(client);

ThreadController threadControl = ThreadController();
Thread thread = Thread();

String s = "";
String weatherData = "";

void topic_subscriber(String topic, String message) {
  Serial.println(s+"Message arrived in function 1 ["+topic+"] "+message);
  weatherData = message;
}

void mqtt_setup() {
  // the stuff we need to do to set up mqtt
  // Connect to MQTT
  Serial.print(s+"Connecting to MQTT: "+MQTT_SERVER+" ... ");
  if (client.connect("ESP8266Client")) {
    Serial.println("connected");

    mqtt.subscribe("/home/basement/BME280", topic_subscriber);
  } else {
    Serial.println(s+"failed, rc="+client.state());
  }

  // Enable Thread
  //thread.onRun(publisher);
  //thread.setInterval(2000);
  //threadControl.add(&thread);
}

void mqtt_loop() {
  // the stuff we need to do for mqtt on each loop
  client.loop();
  //threadControl.run();
}

// Get weather data from MQTT subscription
const char *getTemp() {
  // T63.64,H37.72,P1035.76
  static char buffer[20];
  String tempString = "";
  String humString = "";
  String presString = "";

  if (weatherData.length() != 0)
  {
    Serial.println(s+"weather data: "+weatherData);
    // positions of each data type
    int tempLoc = weatherData.indexOf("T");
    int humLoc = weatherData.indexOf("H");
    int presLoc = weatherData.indexOf("P");
    // parse out each data type
    tempString = weatherData.substring(tempLoc+1, humLoc-1);
    humString = weatherData.substring(hunLoc+1, presLoc-1);
    presString = weatherData.substring(presLoc+1, weatherData.length());
    // format temp
    float tempFloat = round(tempString.toFloat());
    tempString = String((int)tempFloat);
    tempString += char(247);// degree symbol
    // format humidity
    float humFloat = round(humString.toFloat());
    humString = String((int)humFloat);
    humString += char(247);// percent symbol <- find/make
  }
  else {
    tempString = "--";
  }
  Serial.println(s+"temp data: "+tempString);
  Serial.println(s+"hum data: "+humString);
  Serial.println(s+"pres data: "+presString);
  tempString.toCharArray(buffer, tempString.length()+1);
  return buffer;
}

#endif
