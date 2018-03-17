#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <QuickStats.h>

#define LED_PIN 15
#define SENSOR_PIN 0 //analog pin 0

// used for wifi
const char* ssid     = "gopats";
const char* password = "15courthouselane";

ESP8266WebServer server(80);
String webString = "";    // String to display

float val = 0;
float thresh = 70;
int cyclesOn = 0;

// eval stuff
int evalArrayLen = 10;
float evalArray[10];
int evalCtr = -1;
float checkVal = 0;

QuickStats stats; //initialize an instance of class for stats

void setup(){
  Serial.begin(9600);

  // setup LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // do our wifi thing inc. network connect, set hostname, disp html
  wifiSetup();

}

void loop(){
  // for webserver
  server.handleClient();

  /* Do our eval thing */
  if (evalCtr < (evalArrayLen - 1)) {
    evalCtr++;
  }
  else {
    // gather up some values & eval before we delay
    evalCtr = 0;

    processEval();

    delay(500);
  }

  /* Do the read & delay a bit */
  val = analogRead(SENSOR_PIN);
  delay(10);

  Serial.print(evalCtr);
  Serial.print(":");
  Serial.println(val);

  evalArray[evalCtr] = val;

}

/* cycle thru int array, throw out high & low, avg the rest */
void processEval() {

  float result = 0;
/*
  float high = 0;
  float low = 0;
  int highPos = 0;
  int lowPos = 0;

  high = evalArray[0];
  low = evalArray[0];
  highPos = 0;
  lowPos = 0;

  // find the high & low (skip 1st array entry)
  for (int i = 1; i < evalArrayLen; ++i) {
    if (evalArray[i] > high) {
      high = evalArray[i];
      highPos = i;
    }
    else if (evalArray[i] < low) {
      low = evalArray[i];
      lowPos = i;
    }
  }

  Serial.print("highPos:");
  Serial.println(highPos);
  Serial.print("lowPos:");
  Serial.println(lowPos);

  // if high & low are same => all the values are the same =>
  //  don't bother avg-ing
  if (highPos == lowPos) {
    result = evalArray[0];
  }
  else {
    // throw out high & low, avg the array
    for (int i = 0; i < evalArrayLen; ++i) {
      if ((i != highPos) && (i != lowPos)) {
        result = result + evalArray[i];
      }
    }
    result = (result / (evalArrayLen - 2));
  }

  Serial.print("outlierless mean: ");
  Serial.println(result);
*/
  result = stats.mode(evalArray,evalArrayLen,0.1);
  Serial.print("outlierless mode: ");
  Serial.println(result);

  // check to see if evel criteria met
  if (result < thresh) {
    if (digitalRead(LED_PIN) == LOW) {
      digitalWrite(LED_PIN, HIGH);
      // increment cycle on counter
      cyclesOn++;
    }
  }
  else {
    if (digitalRead(LED_PIN) == HIGH) {
      digitalWrite(LED_PIN, LOW);
    }
  }
}

void wifiSetup() {

  // set hostname
  WiFi.hostname(F("giniger"));

  // Setup as softAP
  /*
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");
  */

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print(F("\n\r \n\rWorking to connect"));

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(F("Proximity Display Server"));
  Serial.print(F("Connected to "));
  Serial.println(ssid);
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());

  // Set up mDNS responder:
  //if (!MDNS.begin("giniger")) {
  //  Serial.println("Error setting up MDNS responder!");
  //  while (1) {
  //    delay(1000);
  //  }
  //}
  //Serial.println("mDNS responder started");

  server.on("/", []() {
    webString = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\r\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\
<head>\r\
<title>Proximity Display Server</title>\r\
<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\r\
<meta http-equiv=\"Cache-Control\" content=\"no-cache, no-store, must-revalidate\" />\r\
<meta http-equiv=\"refresh\" content=\"20\" />\r\
<meta name=\"viewport\" content=\"user-scalable=no, initial-scale=1.0, maximum-scale=1.0, width=device-width\" />\r\
<style type=\"text/css\">\r\
body { margin-top:75px; background-color:#ffffff; width:100%; }\r\
h1 { text-align:center; font:normal 24px/0px helveticaneue-bold,sans-serif; color:#666666; text-transform:uppercase; letter-spacing:0.4em; }\r\
h2 { text-align:center; font:normal 48px/0px helveticaneue-ultralight,sans-serif; color:#000000; }\r\
</style>\r\
</head>\r\
<body>\r\
<h1>Voltage</h1>\r<h2>" + String(val, 2) + " mV</h2><br />" + "\r\
<h1>Cycles On</h1>\r<h2>" + String(cyclesOn) + "</h2><br />" + "\r\
</body>\r\
</html>";
    server.send(200, "text/html", webString); // send to someone's browser when asked
  });

  server.begin();
  Serial.println(F("HTTP server started"));

  // Add service to MDNS-SD
  //MDNS.addService("http", "tcp", 80);

}
