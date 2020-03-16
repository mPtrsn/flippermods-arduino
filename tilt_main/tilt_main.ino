#include <WiFi.h>      //ESP32 Core WiFi Library    

#include <WebServer.h> //Local DNS Server used for redirecting all requests to the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )

#include <DNSServer.h> //Local WebServer used to serve the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )
#include <WiFiManager.h>   // WiFi Configuration Magic (  https://github.com/zhouhan0126/DNSServer---esp32  ) >>  https://github.com/zhouhan0126/DNSServer---esp32  (ORIGINAL)
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Base64.h>

WiFiManager wifiManager;

bool debug = false;

String server = "https://api.twistways.com";
String path = "/arduino/1";

char* authString = "Basic YWRtaW46dGVzdA==";
char username[40];
char password[40];

const int PIN_AP = 33;

bool auth = true;
uint16_t sensorId = 1;

WiFiClient wifi;

JSONVar myData;
HTTPClient http;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_AP, INPUT);
  //declaração do objeto wifiManager
  
  
  WiFiManagerParameter custom_username("username", "username", username, 40);
  WiFiManagerParameter custom_password("password", "password", password, 40);

//utilizando esse comando, as configurações são apagadas da memória
  //caso tiver salvo alguma rede para conectar automaticamente, ela é apagada.
  //wifiManager.resetSettings();

 
//callback para quando entra em modo de configuração AP
  wifiManager.setAPCallback(configModeCallback); 
//callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback); 
 
//add all your parameters here
  wifiManager.addParameter(&custom_username);
  wifiManager.addParameter(&custom_password);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //and goes into a blocking loop awaiting configuration
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



  myData["number"] = 1000;
  myData["text"] = "Gruß";
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
  sendData();
}

void sendData() {
     
 
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
//callback que indica que o ESP entrou no modo AP
void configModeCallback (WiFiManager *myWiFiManager) {  
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP()); 
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void saveConfigCallback () {
  Serial.println("Should save config");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(username);
  Serial.println(password);
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
   
