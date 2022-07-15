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
   Serial.begin(9600);

   Serial.println("Ready");
   delay(100);

   PWMDriver.begin();

   //The frequency is set to 60Hz which is equivalent to a PWM signal with 16.6ms period 
   //that is within the duty cycle range of most servos.
   PWMDriver.setPWMFreq(60);

   delay(10);

    TestGCodeParser();


   //PWMDriver.setPWM(JOINT_1, 0, SERVOMIN + (SERVOMAX - SERVOMIN)/2);

   //PWMDriver.setPWM(JOINT_2, 0, SERVOMIN + (SERVOMAX - SERVOMIN)/2);

   //PWMDriver.setPWM(2, 0, PulseLen);
}

void loop() {
   //getSerialString();   

  

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

void TestGCodeParser() {  
   Serial.println("TestGCodeParser");
/*
  To embed a comment in a line, use parentheses; 
  To add a comment to the end of a line, use a semicolon.
  NOTE: The semicolon is not treated as the start of a comment when it’s enclosed in parentheses.

  Comments can appear between words: S100 (set speed) F200 (feed) 

  After the ; that means end of a block, the rest is consdered a comment.

  If a line starts with / or ; means that his entire content will be ignored.

  The lines with only a comment between parentheses denote a message to be shown to the operator
 */  
   //Start program
   ExecuteBlock("%                              "); // Start of program.
   ExecuteBlock("00001 (PROJECT1) (This is more)"); // Program number (Program Name).
   ExecuteBlock("/line ignored                  "); // Program number (Program Name).
   ExecuteBlock("  (T1 0.25 END MILL)             "); // Tool description for operator.
   ExecuteBlock("        N1 (REM1) G17 (REM2) G20 G40 G49 G80 (REM3) G90     "); // Safety block to ensure machine is in safe mode.
   //Change Tool
   ExecuteBlock("  N2 T1 M6;  This is a comment   "); // Load Tool #1.
   ExecuteBlock("N3 S9200 M3                    "); // Spindle Speed 9200 RPM, On CW.
   ExecuteBlock(";Another comment               "); // Spindle Speed 9200 RPM, On CW.
   ExecuteBlock("   ;Another comment            "); // Spindle Speed 9200 RPM, On CW.
  //  //Move to Position
  //  ExecuteBlock("N4 G54                         "); // Use Fixture Offset #1.
  //  ExecuteBlock("N5 M8                          "); // Coolant On.
  //  ExecuteBlock("N6 GOO X-0.025 Y-0.275         "); // Rapid above part.
  //  ExecuteBlock("N7 G43 Z1. H1                  "); // Rapid to safe plane, use Tool Length Offset #1.
  //  ExecuteBlock("N8 Z0.1                        "); // Rapid to feed plane.
  //  ExecuteBlock("N9 GO1 7-0.1 F18.              "); // Line move to cutting depth at 18 IPM.
  //  //Machine Contour
  //  ExecuteBlock("N10 G41 YO.1 D1 F36.           "); // CDC Left, Lead in line, Dia. Offset #1, 36 IPM.
  //  ExecuteBlock("N11 Y2.025                     "); // Line move.
  //  ExecuteBlock("N12 X2.025                     "); // Line move.
  //  ExecuteBlock("N13 Y-0.025                    "); // Line move.
  //  ExecuteBlock("N14 X-0.025                    "); // Line move.
  //  ExecuteBlock("N15 G40 X-0.4                  "); // Turn CDC off with lead-out move.
  //  ExecuteBlock("N16 GOO Z1.                    "); // Rapid to safe plane.
  //  //Change Tool
  //  ExecuteBlock("N17 M5                         "); // Spindle Off.
  //  ExecuteBlock("N18 M9                         "); // Coolant Off.
  //  ExecuteBlock("(T2 0.25 DRILL)                "); // Tool description for operator.
  //  ExecuteBlock("N19 T2 M6                      "); // Load Tool #2.
  //  ExecuteBlock("N20 S3820 M3                   "); // Spindle Speed 3820 RPM, On CW.
  //  //Move to Position
  //  ExecuteBlock("N21 M8                         "); // Coolant On.
  //  ExecuteBlock("N22 X1. Y1.                    "); // Rapid above hole.
  //  ExecuteBlock("N23 G43 Z1. H2                 "); // Rapid to safe plane, use Tool Length Offset 2.
  //  ExecuteBlock("N24 Z0.25                      "); // Rapid to feed plane.
  //  //Drill Hole
  //  ExecuteBlock("N25 G98 G81 Z-0.325 RO.1 F12.  "); // Drill hole (canned) cycle, Depth 2-.325, F12.
  //  ExecuteBlock("N26 G80                        "); // Cancel drill cycle.
  //  ExecuteBlock("N27 Z1.                        "); // Rapid to safe plane.
  //  //End Program
  //  ExecuteBlock("N28 M5                         "); // Spindle Off.
  //  ExecuteBlock("N29 M9                         "); // Coolant Off.
  //  ExecuteBlock("N30 G91 G28 ZO                 "); // Return to machine Home position in Z.
  //  ExecuteBlock("N31 G91 G28 XO YO              "); // Return to machine Home position in XY.
  //  ExecuteBlock("   /Another line ignored       ");
  //  ExecuteBlock("N32 G90                        "); // Reset to absolute positioning mode (for safety).
  //  ExecuteBlock("N33 M30                        "); // Reset program to beginning.
  //  ExecuteBlock("%                              "); // End Program
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

      ExecuteBlock(StrBuffer);
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
   
   //ExecuteBlock(ReceivedData); 
}

bool ExecuteBlock(char Command[100]){

   GCode.ParseLine(Command);

   // Code to process the line of G-Code here…
   if (!GCode.AvoidBlock && !GCode.OperatorMessage) {
      Serial.print("Block : ");
      Serial.println(GCode.line);
      //Serial.print("\t\t\t\t\t");
      //Serial.println(GCode.Comment);
   }  else
   if (GCode.OperatorMessage) {
      Serial.print("Message to the operator : ");
      Serial.println(GCode.line);
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
