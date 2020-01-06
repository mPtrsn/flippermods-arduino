#include <Servo.h>
#define SERVO 10
#define LED 11

Servo servo;
unsigned long servoDelay = 500;
unsigned long ledDelayON = 0;
unsigned long ledDelayOFF = 250;

void setup() {
  servo.attach(SERVO);
  pinMode(LED,OUTPUT);
  randomSeed(analogRead(A1));
}

void loop() {
  if(millis() >= servoDelay){
    int value = random(1, 180);
    servo.write(value);
    servoDelay += 500;
  }
  if(millis() >= ledDelayON){
    digitalWrite(LED, HIGH);
    ledDelayON += 400;
  }
  if(millis() >= ledDelayOFF){
    digitalWrite(LED, LOW);
    ledDelayOFF += 400;
  }
  
}
