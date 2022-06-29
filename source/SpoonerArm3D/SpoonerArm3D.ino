#include <Wire.h>
//#include <PWMServo.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();

#define SERVOMIN 150
#define SERVOMAX 600

uint8_t servonum       = 0;
uint8_t numberOfServos = 2;

void setup() {
   Serial.begin(9600);
   myServo.begin();
   myServo.setPWMFreq(60);
   delay(10);
}

void loop() {
   for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++){
      myServo.setPWM(servonum, 0, pulselen);
   }

   delay(500);
   for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--){
      myServo.setPWM(servonum, 0, pulselen);
   }
   delay(500);
  
   servonum++;
   if (servonum > numberOfServos-1) 
       servonum = 0; 

}

