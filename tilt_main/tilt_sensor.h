#ifndef _TILT_SENSOR_H_
#define _TILT_SENSOR_H_

#include <Arduino_JSON.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>

class TiltSensor {
  public:
    TiltSensor();
    void readData(JSONVar myData);
  private:
    Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);
};



#endif // _TILT_SENSOR_H_
