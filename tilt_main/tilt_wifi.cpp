#include <Arduino.h>
#include <HTTPClient.h>
#include "tilt_wifi.h"

void TiltWifi::setup() {
  Serial.println("TiltWifi Constructor");
  WiFiManager manager;
  wifiManager = manager;
  
  WiFiManagerParameter custom_username("username", "username", username, 40);
  WiFiManagerParameter custom_password("password", "password", password, 40);

  //wifiManager.resetSettings();

  //wifiManager.setAPCallback(configModeCallback); 

  //wifiManager.setSaveConfigCallback(saveConfigCallback); 
 

  wifiManager.addParameter(&custom_username);
  wifiManager.addParameter(&custom_password);

  if (!wifiManager.startConfigPortal("Flippermods_AP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(username, custom_username.getValue());
  strcpy(password, custom_password.getValue());
}

//callback que indica que o ESP entrou no modo AP
    void TiltWifi::configModeCallback (WiFiManager *myWiFiManager) {  
      Serial.println("Entered config mode");
      Serial.println(WiFi.softAPIP()); 
      Serial.println(myWiFiManager->getConfigPortalSSID());
    }

    void TiltWifi::saveConfigCallback () {
      Serial.println("Should save config");
      Serial.println(WiFi.softAPIP()); //imprime o IP do AP
      Serial.println(username);
      Serial.println(password);
    }


void TiltWifi::sendData(JSONVar myData) {
   HTTPClient http;
 
   http.begin("https://api.twistways.com/arduino/1");  //Specify destination for HTTP request
   http.addHeader("Content-Type", "application/json");             //Specify content-type header
   http.addHeader("Authorization", authString);
   http.addHeader("Content-Length", JSON.stringify(myData).length() + "");
   int httpResponseCode = http.POST(JSON.stringify(myData));   //Send the actual POST request

   if(httpResponseCode == 200){
     if(debug){
        String response = http.getString();                       //Get the response to the request
        Serial.println(httpResponseCode);   //Print return code
        Serial.println(response);    
     }
   }else{
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
   }
 
   //http.end();
}
