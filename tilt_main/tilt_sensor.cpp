#include <Arduino.h>
#include "tilt_sensor.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>

TiltSensor::TiltSensor() {
  
  //accel = a343;
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }

  accel.setRange(ADXL343_RANGE_2_G);

  /* Display some basic information on this sensor */
  accel.printSensorDetails();
  Serial.println("");
}


void TiltSensor::readData(JSONVar myData) {
  sensors_event_t event;
  accel.getEvent(&event);
myData["number"] = event.acceleration.x + event.acceleration.y;
  
}
