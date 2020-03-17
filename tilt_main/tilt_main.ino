
#include "tilt_const.h"
#include "tilt_sensor.h"
#include "tilt_wifi.h"

#define PIN_AP 32
#define PIN_SENSOR 35

WiFiManager wifiManager;
WiFiClient wifi;
JSONVar myData;
HTTPClient http;

bool debug = false;

TiltWifi tiltWifi;
TiltSensor tiltSensor;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_AP, INPUT);
  pinMode(PIN_SENSOR, INPUT);
  pinMode(14, OUTPUT);

  tiltWifi.setup();
  myData["number"] = 1000;
  myData["text"] = "Gruß";
}


void loop() {
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
  tiltSensor.readData(myData);
  tiltWifi.sendData(myData);
}



   
