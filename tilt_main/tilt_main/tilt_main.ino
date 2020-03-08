#include <ArduinoJson.h>


#include <WiFi.h>      //ESP32 Core WiFi Library    

#include <WebServer.h> //Local DNS Server used for redirecting all requests to the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )

#include <DNSServer.h> //Local WebServer used to serve the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )
#include <WiFiManager.h>   // WiFi Configuration Magic (  https://github.com/zhouhan0126/DNSServer---esp32  ) >>  https://github.com/zhouhan0126/DNSServer---esp32  (ORIGINAL)
#include <ArduinoHttpClient.h>

WiFiManager wifiManager;

char *server = "192.168.178.53";
uint16_t port = 6565;

char username[40];
char password[40];

const int PIN_AP = 33;

bool auth = true;
uint16_t sensorId = 1;

WiFiClient wifi;

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
   // send Data
  sendData();
}

void sendData() {
  client.begin();
  if (client.connect(server, port)) {
    Serial.println("ES HAT GEKLAPPT!");
        // TODO id etc bekommen
      client.println("GET /test HTTP/1.1");
      client.println();
    } else {
      Serial.println("Failed to connect to server");
    }
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
}
   
