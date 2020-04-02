#include <WiFi.h>      //ESP32 Core WiFi Library    

#include <WebServer.h> //Local DNS Server used for redirecting all requests to the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )

#include <DNSServer.h> //Local WebServer used to serve the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )
#include <WiFiManager.h>   // WiFi Configuration Magic (  https://github.com/zhouhan0126/DNSServer---esp32  ) >>  https://github.com/zhouhan0126/DNSServer---esp32  (ORIGINAL)
#include <WebSocketsClient.h>
#include "StompClient.h"
#include <Arduino_JSON.h>


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>

#define SERIAL Serial


WiFiManager wifiManager;
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);

bool debug = false;

bool useWSS                       = false;
const char* ws_host               = "192.168.178.51";
const int ws_port                 = 6565;
const char* ws_baseurl            = "/a/";

char* authString = "Basic YWRtaW46dGVzdA==";
char username[40];
char password[40];

const int PIN_AP = 32;

bool auth = true;
uint16_t sensorId = 1;

WiFiClient wifi;

JSONVar myData;

WebSocketsClient webSocket;

Stomp::StompClient stomper(webSocket, ws_host, ws_port, ws_baseurl, true);

void setup() {
  Serial.begin(9600);
  pinMode(PIN_AP, INPUT);
  pinMode(35, INPUT);
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);

  setupWifi();
  setupWebsocket();
  setupSensor();
  myData["number"] = 1000;
  myData["id"] = 1;
}


void loop() {
/*
  //resetButton
   if ( digitalRead(PIN_AP) == HIGH ) {
      Serial.println("restart"); //tenta abrir o portal
      if(!wifiManager.startConfigPortal("Flippermods_AP") ){
        Serial.println("Failed to Connect");
        delay(2000);
        ESP.restart();
        delay(1000);
      }
      Serial.println("Connected to AP!!!");
   }
*/

// check username
  // if present -> make base64 authstring
   // send Data
   webSocket.loop();
   sendData();
}

void setupWifi(){
  WiFiManagerParameter custom_username("username", "username", username, 40);
  WiFiManagerParameter custom_password("password", "password", password, 40);

  //wifiManager.resetSettings();

 

  wifiManager.setAPCallback(configModeCallback); 

  wifiManager.setSaveConfigCallback(saveConfigCallback); 
 

  wifiManager.addParameter(&custom_username);
  wifiManager.addParameter(&custom_password);

  
  if (!wifiManager.startConfigPortal("Flippermods_AP")) {
    SERIAL.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  SERIAL.println("connected...yeey :)");

  //read updated parameters
  strcpy(username, custom_username.getValue());
  strcpy(password, custom_password.getValue());
}

void setupWebsocket(){
  stomper.onConnect(subscribe);
  stomper.onError(error);

  // Start the StompClient
  if (useWSS) {
    stomper.beginSSL();
  } else {
    stomper.begin();
  }
}
// Once the Stomp connection has been made, subscribe to a topic
void subscribe(Stomp::StompCommand cmd) {
  Serial.println("Connected to STOMP broker");
}

void error(const Stomp::StompCommand cmd) {
  Serial.println("ERROR: " + cmd.body);
}

void setupSensor() {
  //accel = a343;
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    SERIAL.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }

  accel.setRange(ADXL343_RANGE_2_G);

  /* Display some basic information on this sensor */
  accel.printSensorDetails();
  SERIAL.println("");
}

void sendData() {
  sensors_event_t event;
  accel.getEvent(&event);
  /*int x = event.acceleration.x;
  int y = event.acceleration.y;
  if (x < 0 ) x *= -1;
  if (y < 0 ) y *= -1;
  int res = x + y;
  int mapped = map(res, 0, 5, 0, 1000);
  myData["number"] = map(res, 0, 5, 0, 1000);
  */
 
  stomper.sendMessage("/app/data/1", "{\\\"id\\\":\\\"1\\\",\\\"number\\\":500}");//JSON.stringify(myData)
}
//callback que indica que o ESP entrou no modo AP
void configModeCallback (WiFiManager *myWiFiManager) {  
  SERIAL.println("Entered config mode");
  SERIAL.println(WiFi.softAPIP()); 
  SERIAL.println(myWiFiManager->getConfigPortalSSID());
}

void saveConfigCallback () {
  SERIAL.println("Should save config");
  SERIAL.println(WiFi.softAPIP()); //imprime o IP do AP
  SERIAL.println(username);
  SERIAL.println(password);
/*
// encoding
  char *inputString = strcat(username, password);
  int inputStringLength = sizeof(inputString);

  Serial.print("Input string is:\t");
  Serial.println(inputString);

  Serial.println();

  int encodedLength = Base64.encodedLength(inputStringLength);
  char encodedString[encodedLength];
  Base64.encode(encodedString, inputString, inputStringLength);
  Serial.print("Encoded string is:\t");
  Serial.println(encodedString);
  */
}
   
