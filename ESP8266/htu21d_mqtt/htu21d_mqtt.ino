/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_HTU21DF.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#define CONFIG_FILE  "/config.json"
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_HTU21DF htu = Adafruit_HTU21DF(); // I2C

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
char result[19] = "T"; // T[+/-]xx.x,Hyy.y,Pzz.z <-- greatest length: 19
// used to build published data - char[]
char loctemp [6]; // [+/-]xx.x
char lochum [5]; // xx.x

//MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long delayTime = 1000;

void setup() {
    Serial.begin(115200);
    Serial.println(F("HTU21D test"));

    // read config to wifi/mqtt/whatever config data
    readConfig();

    // do our wifi thing inc. network connect, set hostname
    wifiSetup();

    // *** do OTA stuff
    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // default settings
    bool status = htu.begin();
    if (!status) {
        Serial.println("Could not find a valid HTU21D sensor, check wiring!");
        while (1);
    }

    Serial.println();

    // setup MQTT
    client.setServer(mqtt_server, mqtt_port);
}

void loop() {
    // *** do OTA stuff
    ArduinoOTA.handle();

    printValues();
    delay(delayTime);

    // MQTT publish
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    // Publish data
    pubData(tempToF(htu.readTemperature()), htu.readHumidity(), 0);
}

void reconnect() {
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

void pubData(float temp, float humidity, float pressure) {
  // publish real data
  //Serial.print("MQTT client state:");
  //Serial.println(client.state());

  // convert temp to string
  dtostrf(temp, 4, 1, loctemp);
  // convert humidity to string
  dtostrf(humidity, 4, 1, lochum);

  // DEBUG
  //Serial.print("loctemp: ");
  //Serial.println(loctemp);
  //Serial.print("lochum: ");
  //Serial.println(lochum);

  // -- char[] way
  // T[+/-]xx.x,Hyy.y,Pzz.z <-- greatest length: 19

  // (re)init array
  result[1] = '\0';

  strcat(result, loctemp);
  strcat(result, ",H");
  strcat(result, lochum);
  // Don't bother w/ pressure --> we don't have it

  // DEBUG
  Serial.print("result: ");
  Serial.println(result);

  client.publish(mqtt_channel, result);
}

// convert temperature
float tempToF(float tempC) {
  return (tempC * 9.0F)/5.0F + 32.0F;
}

// convert pressure
float pressToMBar(float pressure) {
  return (pressure / 100.0F);
}

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(tempToF(htu.readTemperature()));
    Serial.println(" *F");

    Serial.print("Humidity = ");
    Serial.print(htu.readHumidity());
    Serial.println(" %");

    Serial.println();
}

void wifiSetup() {
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

  Serial.println(F("HTU21D Environmental Display Server"));
  Serial.print(F("Connected to "));
  Serial.println(ssid);
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("hostname: "));
  Serial.println(WiFi.hostname());
}

// file io
File GetFile(String fileName) {
  File textFile;
  if (SPIFFS.exists(fileName)) {
    textFile = SPIFFS.open(fileName, "r");
  }
  return textFile;
}

void readConfig() {
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