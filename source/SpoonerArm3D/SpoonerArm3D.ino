#include <Wire.h>
//#include <PWMServo.h>
#include <Adafruit_PWMServoDriver.h>  //PCA9685 servo motor driver.

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();

//These values are dependant of the servo manufacurer
#define SERVOMIN 150  //  0º position
#define SERVOMAX 600  //180º position

uint8_t CurServo       = 0; //Current Servo
uint8_t numberOfServos = 3;

void setup() {
   Serial.begin(9600);
   myServo.begin();

   //The frequency is set to 60Hz which is equivalent to a PWM signal with 16.6ms period 
   //that is within the duty cycle range of most servos.
   myServo.setPWMFreq(60);

   delay(10);

   myServo.setPWM(0, 0, pulselen);

   myServo.setPWM(1, 0, pulselen);
}

void loop() {

  //The function setPWM(n_servo, on, off) is used to set the pulse width of a PWM output.
  //  n_servo :the number of servo or output to be configured (0 to 15)
  //  on      :the value the PWM signal rising edge, that is, when the signal should transition from low to high. 
  //           Since it is 12-bit signal, the value should be between 0 to 4095.
  //  off     :the value of PWM signal falling edge, that is, when the signal should transition 
  //           from high to low and the value ​​​​must also be between 0 and 4096.

   for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++){
      myServo.setPWM(2, 0, pulselen);
      //delay(5);
   }
   delay(500);

   for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--){
      myServo.setPWM(2, 0, pulselen);
      delay(2);
   }
   delay(500);
  
   CurServo++;
   if (CurServo > numberOfServos -1) {
      CurServo = 0; 
   }   

}

