#include <string.h>
#include <Wire.h>
//#include <PWMServo.h>
#include <GCodeParser.h>
#include <Adafruit_PWMServoDriver.h>  //PCA9685 servo motor driver.

Adafruit_PWMServoDriver PWMDriver = Adafruit_PWMServoDriver();
GCodeParser             GCode     = GCodeParser();

//These values are dependant of the servo manufacurer
#define SERVOMIN    150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX    600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN       600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX      2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ   50 // Analog servos run at ~50 Hz updates

#define JOINT_1 0
#define JOINT_2 1
#define JOINT_3 2

#define LF 0x0A  //Hexadecimal representation of LineFeed character

uint8_t CurServo       = 0; //Current Servo
uint8_t numberOfServos = 3;

uint16_t PulseLen = SERVOMIN;

void setup() {
   //Serial.begin(9600);
   //Serial.println(millis());

   Serial.begin(115200);

   Serial.println("Ready");
   delay(100);

   PWMDriver.begin();

   //The frequency is set to 60Hz which is equivalent to a PWM signal with 16.6ms period 
   //that is within the duty cycle range of most servos.
   PWMDriver.setPWMFreq(60);

   delay(10);

   //PWMDriver.setPWM(JOINT_1, 0, SERVOMIN + (SERVOMAX - SERVOMIN)/2);

   //PWMDriver.setPWM(JOINT_2, 0, SERVOMIN + (SERVOMAX - SERVOMIN)/2);

   //PWMDriver.setPWM(2, 0, PulseLen);
}

void loop() {
   getSerialString();   


  //The function setPWM(n_servo, on, off) is used to set the pulse width of a PWM output.
  //  n_servo :the number of servo or output to be configured (0 to 15)
  //  on      :the value the PWM signal rising edge, that is, when the signal should transition from low to high. 
  //           Since it is 12-bit signal, the value should be between 0 to 4095.
  //  off     :the value of PWM signal falling edge, that is, when the signal should transition 
  //           from high to low and the value ​​​​must also be between 0 and 4096.
   
   
   
  //  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++){
  //     myServo.setPWM(2, 0, pulselen);
  //     //delay(5);
  //  }
  //  delay(500);

  //  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--){
  //     myServo.setPWM(2, 0, pulselen);
  //     delay(2);
  //  }
  //  delay(500);
  
  //  CurServo++;
  //  if (CurServo > numberOfServos -1) {
  //     CurServo = 0; 
  //  }   

}

void getSerialString() {
   char StrBuffer[100];
   int i = 0;  

   //ensure the buffer is empty.
   //memset(StrBuffer, 0, sizeof(StrBuffer));
   StrBuffer[0] = 0;

   while (Serial.available()) {
      delay(10);  //small delay to allow input buffer to fill
      StrBuffer[i] = Serial.read();
      if (StrBuffer[i] == LF) {
         StrBuffer[i] = 0; //Inserts the \0 character as end of the string
         i = -1;
      }
      i++;
   }

   if(strlen(StrBuffer) != 0) {
      Serial.print("Received Data : ");
      Serial.println(StrBuffer);
      ExecuteCommand(StrBuffer);
   };

   
}
 /* -------------------------------------------------
  |              getSerialCommands()                |
   -------------------------------------------------
  |   Receibes commans throught the serial port     |
  | Normaly text commands from Serial Monitor       |
  ------------------------------------------------- */
void getSerialCommands() {
  
   int ReceivedData = -1;
  
   if (Serial.available() > 0) {
     String str = Serial.readStringUntil('\n');
     ReceivedData = str.toInt();
     Serial.print("Received Data : ");
     Serial.println(ReceivedData);
   }
   
   //ExecuteCommand(ReceivedData); 
}

bool ExecuteCommand(char Command[100]){
   //if (Command == "0") PosJoint1ToHome(); else 
   //if (Command == "1") PosJoint1ToMax();  else
   //if (Command == "2") Serial.println("Command Two"); else
   //if (Command == "3") Serial.println("Command Three"); 
   //else Serial.println("Another Command");  

  GCode.ParseLine(Command);

  // Code to process the line of G-Code here…
  Serial.print("Command Line: ");
  Serial.println(GCode.line);

  GCode.RemoveCommentSeparators();

  Serial.print("Comment(s): ");
  Serial.println(GCode.comments);
      
  if (GCode.HasWord('G')) {
     Serial.print("Process G code: ");
     Serial.println((int)GCode.GetWordValue('G'));
  }
 }


void PosJoint1ToHome(){
   PWMDriver.setPWM(JOINT_1, 0, SERVOMIN);
}

void PosJoint1ToMax(){
   PWMDriver.setPWM(JOINT_1, 0, SERVOMAX);
}

void SetPosServo(int Servo, int Degrees) {
     Degrees = map(Degrees, SERVOMIN, SERVOMAX, 0, 180);
     PWMDriver.setPWM(Servo, 0, Degrees);
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulseLength;
  
  pulseLength = 1000000;      // 1,000,000 us per second
  pulseLength /= SERVO_FREQ;  // Analog servos run at ~60 Hz updates
  Serial.print("Micro Seconds per period\t"); Serial.println(pulseLength); 
  pulseLength /= 4096;  // 12 bits of resolution
  Serial.print("Microseconds per bit\t"); Serial.println(pulseLength); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulseLength;
  Serial.println(pulse);
  PWMDriver.setPWM(n, 0, pulse);
}
