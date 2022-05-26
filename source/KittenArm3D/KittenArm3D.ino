#include <Wire.h>
#include <PWMServo.h>



//PWMServo servos = PWMServo(Direccion);

#define TIME_WAIT 300

void setup() {
   pinMode(LED_BUILTIN, OUTPUT);


}

void loop() {
   
   digitalWrite(LED_BUILTIN, HIGH);
   delay(TIME_WAIT);
   digitalWrite(LED_BUILTIN, LOW);
   delay(TIME_WAIT);  

}
