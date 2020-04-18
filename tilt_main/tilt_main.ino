#include <WiFi.h>      //ESP32 Core WiFi Library    

#include <WebServer.h> //Local DNS Server used for redirecting all requests to the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )

#include <DNSServer.h> //Local WebServer used to serve the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )
#include <WiFiManager.h>   // WiFi Configuration Magic (  https://github.com/zhouhan0126/DNSServer---esp32  ) >>  https://github.com/zhouhan0126/DNSServer---esp32  (ORIGINAL)
#include <WebSocketsClient.h>
#include "StompClient.h"
#include <Arduino_JSON.h>
#include <HTTPClient.h>


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>

#define SERIAL Serial

const char* cert= \
"-----BEGIN CERTIFICATE-----"
"MIIGWzCCBUOgAwIBAgISA2/SmowhVWoyZoG1F2Hid2WdMA0GCSqGSIb3DQEBCwUA\n" \
"MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD\n" \
"ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0yMDAyMjgxODAwMTNaFw0y\n" \
"MDA1MjgxODAwMTNaMBwxGjAYBgNVBAMTEWFwaS50d2lzdHdheXMuY29tMIICIjAN\n" \
"BgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAyYKeitjEnX/iCRe1uXvrkzA9BnTE\n" \
"SIcJouaztoTY3JKh39AHPjJbwapW4+JxMVkWEOyF3S4tt3kWKb1oQ2dohY5uZEyC\n" \
"loEIWkr/A0+B0iCxiHL5hB+1idc3yEyI96vLechXXeyF3V4H+Q0KcCSCCg7LeVZh\n" \
"GJZYZ6ZQ7vfPhIoKF3IxH3d7OYtozGJOPNY5qOJ72QDW9UQLDm129OxMwtY5fKHE\n" \
"W2oqyfKgxZ37FsCoi1hx4pQ0MDH1lZL4ktC7URwpIVEfvOWcurhRrwqSZyiNkJKr\n" \
"lbrm5/vTi4mhWAO1EYyYdcu4c7zxxCuz72AcNcNfhpF8FJgK3UIrSwUlZbraAjqW\n" \
"DjagFIWlwH62xPI/Wt5SuHdiTsTh7SLBfGMoJnImseiL86z8ijFTLOy6Suol+4tb\n" \
"50pUDxb/FxRfIw3pVueZtt2wbJ6MT1X/dYFPIGKu+dqbk8/NOAMH4nZqFx7sTF8H\n" \
"LJu1KLNFpP4wo5alLstgRFc02tG/QpyOS/U5ff6uTqMFmvTuVWSwv6EmBVq3kjI6\n" \
"evxEgQsTbBUVuoI2IRLYEwfC2sc/xl1by0wxiymEuIgsui4Oc3G2S99yGqN9fOVA\n" \
"O232WR71jKGFPGBptRz9z4wR0sLGoFbzYVvTDeuEQEl3qPay5ohYFt3v2zrH6A69\n" \
"HEkz2ITlyYQ9vicCAwEAAaOCAmcwggJjMA4GA1UdDwEB/wQEAwIFoDAdBgNVHSUE\n" \
"FjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQU\n" \
"tiXZBXf/0cvUPbCNtGOdo1pMcGswHwYDVR0jBBgwFoAUqEpqYwR93brm0Tm3pkVl\n" \
"7/Oo7KEwbwYIKwYBBQUHAQEEYzBhMC4GCCsGAQUFBzABhiJodHRwOi8vb2NzcC5p\n" \
"bnQteDMubGV0c2VuY3J5cHQub3JnMC8GCCsGAQUFBzAChiNodHRwOi8vY2VydC5p\n" \
"bnQteDMubGV0c2VuY3J5cHQub3JnLzAcBgNVHREEFTATghFhcGkudHdpc3R3YXlz\n" \
"LmNvbTBMBgNVHSAERTBDMAgGBmeBDAECATA3BgsrBgEEAYLfEwEBATAoMCYGCCsG\n" \
"AQUFBwIBFhpodHRwOi8vY3BzLmxldHNlbmNyeXB0Lm9yZzCCAQUGCisGAQQB1nkC\n" \
"BAIEgfYEgfMA8QB2APCVpFnyANGCQBAtL5OIjq1L/h1H45nh0DSmsKiqjrJzAAAB\n" \
"cI0rqmEAAAQDAEcwRQIhALa8bQVOMf0D2+aouS3dd0NKTjL9T11Za7DAssvi5sEQ\n" \
"AiBZjpS4UHTi/pJ5Kcd2RzTPeL9ZbBgKymFUJ/BLdo7bkwB3ALIeBcyLos2KIE6H\n" \
"ZvkruYolIGdr2vpw57JJUy3vi5BeAAABcI0rqngAAAQDAEgwRgIhAPJGsdIWuoUT\n" \
"m2q9gc0VrGaHLlZibQxHVswMfcxzlhhIAiEAqMbfGW8x4E6GsUPebtDCBXDbqYQe\n" \
"0rMsSueetgdrCdAwDQYJKoZIhvcNAQELBQADggEBADOtH7KPE6ixx1WsKlO36or1\n" \
"nrIJmI6yp8gX6mjr0TTzdUagVcuY3Gl0FwyH6a+zIpiE/Dyk8ajyHthD54Xd6qTx\n" \
"BX8DvAT31j9j09K8A9fRGJ7BFHIjtge/XC6DfzbxlBQcRAcGRdYZU+9/TZuCV5Ob\n" \
"3y7b41hjYpFZgUU4AX5fmMNFacW+nBjCuHUb4QIoBNefp4LbBarzWnJ4EctZlxFE\n" \
"hULhgVe+BoMw5ZtUJlhrkH/SUyCKgqxYxgd6D37XDKPMBv8gYwp6aI21L2f3XAal\n" \
"l0rE1R49bcJETWbAhHS4rLHjVCWqQLBu02Ta1iDqCk77qVIXobpgOlJhJer+XsI=\n" \
"-----END CERTIFICATE-----\n";

WiFiManager wifiManager;
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);

bool debug = true;

bool useWSS                       = false;
const char* ws_host               = "api.twistways.com";
const int ws_port                 = 6565;
const char* ws_baseurl            = "/a/";

char* authString = "Basic YWRtaW46dGVzdA==";
char username[40];
char password[40];

const int PIN_AP = 32;

bool auth = true;
int sensorId = -1;

WiFiClient wifi;

HTTPClient http;

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
if(sensorId > 0){
  sendData();
} else {
  getUserId();
}
   webSocket.loop();
   
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

void getUserId(){
  Serial.println("Logging in User");
  if (username != "" && password != ""){
    JSONVar userData;
    userData["username"] = username;
    userData["passwordMD5"] = password;
    http.begin("https://api.twistways.com/user/login", cert);  //Specify destination for HTTP request  e5bd3b47c1cd58f0de0cda30a472b70d408fbbff
    http.addHeader("Content-Type", "application/json");             //Specify content-type header
    http.addHeader("Authorization", authString);
    http.addHeader("Content-Length", JSON.stringify(userData).length() + "");
    int httpResponseCode = http.POST(JSON.stringify(userData));   //Send the actual POST request

    if(httpResponseCode == 200){
       String response = http.getString();                       //Get the response to the request
       Serial.println(httpResponseCode);   //Print return code
       Serial.println(response);    
       JSONVar myObject = JSON.parse(response);
       sensorId = (int)myObject["id"];
      
   }else{
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
   }
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
  int x = event.acceleration.x;
  int y = event.acceleration.y;
  if (x < 0 ) x *= -1;
  if (y < 0 ) y *= -1;
  int res = x + y;
  int mapped = map(res, 0, 5, 0, 1000);
  int tiltValue = map(res, 0, 5, 0, 1000);
  
 
  stomper.sendMessage("/app/data/"+String(sensorId), "{\\\"id\\\":\\\""+String(sensorId)+"\\\",\\\"number\\\":"+String(tiltValue)+"}");
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
   
