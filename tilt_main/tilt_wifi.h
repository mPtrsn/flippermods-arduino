#ifndef _TILT_WIFI_H_
#define _TILT_WIFI_H_

#include <Arduino.h>
#include <WiFi.h>      //ESP32 Core WiFi Library    

#include <WebServer.h> //Local DNS Server used for redirecting all requests to the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )

#include <DNSServer.h> //Local WebServer used to serve the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )
#include <WiFiManager.h>   // WiFi Configuration Magic (  https://github.com/zhouhan0126/DNSServer---esp32  ) >>  https://github.com/zhouhan0126/DNSServer---esp32  (ORIGINAL)
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Base64.h>

class TiltWifi {
  public:
    void setup();
    void sendData(JSONVar myData);
    void checkReset();
    void configModeCallback (WiFiManager *myWiFiManager);
    void saveConfigCallback ();
  private:
    char* authString = "Basic YWRtaW46dGVzdA==";
    char username[40];
    char password[40];
    bool auth = true;
    const bool debug = false;
    WiFiManager wifiManager;


};

#endif // _TILT_WIFI_H_
