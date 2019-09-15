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

// MQTT defines
#define MQTT_SERVER "bigasspi.fios-router.home"
#define MQTT_CLIENT "ESP8266Client"
#define MQTT_SUB "/home/basement/BME680"

// compensation factors - sensor too close to house?
#define COMP_TEMP -7
// bounds to determine pressure change
#define P_RADIP_CHANGE 6.1
#define P_CHANGE .1

WiFiClient espClient;
PubSubClient client(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(client);

// only need this for publishing?
//ThreadController threadControl = ThreadController();
//Thread thread = Thread();

String s = "";
String weatherData = "";

// pressure stuff
float lastPressure = 0;
String lastIndicator = "--";

void topic_subscriber(String topic, String message) {
  Serial.println(s+"Message arrived in function 1 ["+topic+"] "+message);
  weatherData = message;
}

void mqtt_setup() {
  // the stuff we need to do to set up mqtt
  // Connect to MQTT
  Serial.print(s+"Connecting to MQTT: "+MQTT_SERVER+" ... ");
  if (client.connect(MQTT_CLIENT)) {
    Serial.println("connected");

    mqtt.subscribe(MQTT_SUB, topic_subscriber);
    Serial.println("MQTT subscribe successful");
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
const char *getWeather(bool doPChange) {
  // T63.64,H37.72,P1035.76
  static char buffer[20];
  String tempString = "";
  String humString = "";
  String presString = "";
  String presIndicator = "--";

  Serial.println(s+"weather data 1: "+weatherData);
  if (weatherData.length() != 0) {
    Serial.println(s+"weather data 2: "+weatherData);
    // positions of each data type
    int tempLoc = weatherData.indexOf("T");
    int humLoc = weatherData.indexOf("H");
    int presLoc = weatherData.indexOf("P");
    // parse out each data type
    tempString = weatherData.substring(tempLoc+1, humLoc-1);
    humString = weatherData.substring(humLoc+1, presLoc-1);
    presString = weatherData.substring(presLoc+1, weatherData.length());
    // format temp - apply compensation
    float tempFloat = round(tempString.toFloat()) + COMP_TEMP;
    tempString = String((int)tempFloat);
    tempString += char(96);// degree symbol
    //tempString += char(0x7E);// degree symbol
    // format humidity
    float humFloat = round(humString.toFloat());
    humString = String((int)humFloat);
    //humString += char(37);// percent symbol
    humString += char(0x25);// percent symbol
    // format pressure
    float pressFloat = presString.toFloat();

    // let's make a pressure indicator
    if (lastPressure != 0) {
      //set initial pressure
      lastPressure = pressFloat;
    }
    else if (doPChange) {
      float pressureChange = abs((pressFloat - lastPressure));
      if (pressureChange >= P_RADIP_CHANGE) {
        //changing rapidly
        Serial.println("pressure changing rapidly");
      }
      else if (pressureChange >= P_CHANGE) {
        //changing slowly
        Serial.println("pressure changing slowly");
      }
      else {
        //steady
        Serial.println("pressure steady");
      }
    }

    // build weather
    //tempString = tempString + " " + humString + " " +presString+char(0x7C);
    tempString = tempString + " " + humString + " " +presString+lastIndicator;

    Serial.println(s+"temp data: "+tempString);
    Serial.println(s+"hum data: "+humString);
    Serial.println(s+"pres data: "+presString);
    tempString.toCharArray(buffer, tempString.length()+1);
    //return buffer;
  }
return buffer;
}

#endif
