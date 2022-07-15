/*
MIT License

Copyright (c) 2022 Juan Carlos Cilleruelo Gonzalo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
Special character codes:
   % Begins or ends a CNC program
   () Defines a comment written by a CNC operator; occasionally, these must be in all caps
   / Ignores all characters that come after the slash
   ; Determines when a block of code ends, not shown in a text editor
*/

#include "GCodeParser.h"
#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stdio.h>





 /// <summary>
 /// Initializizes class.
 /// </summary>
void GCodeParser::Initialize()
{
   line[0]         = '\0';
   AvoidBlock      = false;
   OperatorMessage = false;
}

/// <summary>
/// Class constructor.
/// </summary>
/// <remark>
/// The G Code language is based on the RS274/NGC language. The G Code 
/// language is based on lines of code. Each line (also called a 'block')
/// may include commands to do several different things. Lines of code
/// may be collected in a file to make a program.
/// </remark>
GCodeParser::GCodeParser()
{
   Initialize();
}

/// <summary>
/// Parses the line passed removing spaces, tabs and comments. Comments are moved to a new variable.
/// </summary>
void GCodeParser::ParseLine(char* Value)
{
   char* token;
   //Serial.println("Parsing Line");

   int CurPosition = 0;

   Initialize();

   int i = 0;
   while (Value[i] != '\0') {
      line[i] = Value[i];
      i++;
   }
  	
   line[i] = '\0';
   
   ProcessComments();
   if (!AvoidBlock && !OperatorMessage) {
      while(CurPosition < strlen(line)) {
         token = GetToken(CurPosition, line);
         if(IsGCodeCommand(token)) {
            GetGCodeNumber(token);
            Serial.print("Token : " ); Serial.println(token);

         }
         
      }   
   }   
}

/*
   Parser functions 
*/

// Parsing the input STRING.
char* GCodeParser::GetToken(int &left, char* str)
{
   int right = left;
   int end   = strlen(str);

   while(isDelimiter(str[left]) && left < end) {
      left++;
   }
   right = left;

   while (!isDelimiter(str[right]) && right < end) {  
      right++;
   }

   char* token = subString(str, left, right - 1);
   left = right; 
   return token;
}

// Returns 'true' if the string is a Valid GCode command.
bool GCodeParser::IsGCodeCommand(char* str)
{
   //Only Malin valid GCodes. 
   return (str[0] == 'G' || str[0] == 'M' || str[0] == 'T');
}

int GCodeParser::GetGCodeNumber(char* GCommand)
{
 

}   

// Returns 'true' if the character is a DELIMITER.
bool GCodeParser::isDelimiter(char ch)
{
    return (ch == ' ' || ch == '\t');
}
 
// Returns 'true' if the character is an OPERATOR.
bool GCodeParser::isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == '>' || ch == '<' ||
            ch == '=');
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool GCodeParser::validIdentifier(char* str)
{
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || isDelimiter(str[0]) == true)
        return (false);
    return (true);
}
 
// Returns 'true' if the string is a KEYWORD.
bool GCodeParser::isKeyword(char* str)
{                                    // https://marlinfw.org/docs/gcode/G000-G001.html
    if (!strcmp(str, "G0"      ) ||  // Linear Move (without strusion)
        !strcmp(str, "G1"      ) ||  // LInear Move (with strusion)
        !strcmp(str, "G2"      ) ||  // Clockwise arc move.
        !strcmp(str, "G3"      ) ||  // counter-clockwise arc move.
        !strcmp(str, "G4"      ) ||  // Dwell  (delay)
        !strcmp(str, "G5"      ) ||  // Beziers cubic spline 
        !strcmp(str, "G6"      ) ||  // Direct stepper Move
        //!strcmp(str, "G10"     ) ||  // Retract filament
        //!strcmp(str, "G11"     ) ||  // Unretract filament 
        //!strcmp(str, "G12"     ) ||  // Starts the nozzle cleaning process
        !strcmp(str, "G17"     ) || // Select CNC workspace plane XY
        !strcmp(str, "G18"     ) || // Select CNC workspace plane ZX
        !strcmp(str, "G19"     ) || // Select CNC workspace plane YZ
        !strcmp(str, "G20"     ) || // Set units to inches 
        !strcmp(str, "G21"     ) || // Set units to milimiters  
        !strcmp(str, "G26"     ) || // Test the mesh and adjust.
        !strcmp(str, "G27"     ) || // Park the current toolhead
        !strcmp(str, "G28"     ) || // Auto home one or more axes.
        !strcmp(str, "G29"     ) || // Probe the bed and enable leveling compensation.
        !strcmp(str, "G30"     ) || // Probe bed at current XY location
        !strcmp(str, "G31"     ) || // Dock the Z probe sled.
        !strcmp(str, "G32"     ) || // Undock the Z probe sled.
        !strcmp(str, "G33"     ) || // Calibrate various Delta parameters
        !strcmp(str, "G34"     ) || // Modern replacement for Průša's TMC_Z_CALIBRATION
        !strcmp(str, "G34"     ) || // Align multiple Z steppers using a bed probe
        !strcmp(str, "G35"     ) || // Run a procedure to tram the bed
        !strcmp(str, "G38.2"   ) || // Probe towards (or away from) a workpiece
        !strcmp(str, "G38.5"   ) || // 
        !strcmp(str, "G42"     ) || // Move to a specific point in the leveling mesh
        !strcmp(str, "G53"     ) || // Apply native workspace to the current move.
        !strcmp(str, "G54"     ) || // Select a workspace coordinate system
        !strcmp(str, "G59.3"   ) || //
        !strcmp(str, "G60"     ) || // Save current position to specified slot
        !strcmp(str, "G61"     ) || // Return to saved position of specified slot
        !strcmp(str, "G76"     ) || // Calibrate probe temperature compensation
        !strcmp(str, "G80"     ) || // Cancel the current motion mode
        !strcmp(str, "G90"     ) || // Set the interpreter to absolute positions
        !strcmp(str, "G91"     ) || // Set the interpreter to relative positions
        !strcmp(str, "G92"     ) || // Set the current position of one or more axes.
        !strcmp(str, "G425"    ) ) // Use a conductive object to calibrate XYZ backlash
        return (true);
    return (false);
}
 
/*
M0-M1 - Stop and wait for user
M3 - Set the spindle CW speed or laser power
M4 - Set the spindle CCW speed or laser power
M5 - Turn off spindle or laser
M7-M9 - Turn mist or flood coolant on / off
M10-M11 - Enable and disable the Cutter Vacuum or Laser Blower Motor
M16 - Prevent G-code usage on the wrong machine
M17 - Enable steppers
M18, M84 - Disable steppers (same as M84).
M20 - List the contents of the SD Card.
M21 - Attempt to detect an SD card in the slot.
M22 - Simulate ejection of the SD card
M23 - Select an SD file to be executed
M24 - Start or resume a file selected with [`M23`](/docs/gcode/M023.html)
M25 - Pause printing from the SD card
M26 - Set the SD read position
M27 - Print SD progress to serial
M28 - Start writing to a file on the SD card
M29 - Stop writing the file, end logging.
M30 - Delete a specified file from SD.
M31 - Report the current print time.
M32 - Begin an SD print from a file.
M33 - Convert a short pathname to a long pathname.
M34 - Set SDCard file sorting options. 
M42 - Set an analog or digital pin to a specified state.
M43 T - Get information about pins. 
M43 - Get information about pins.
M48 - Measure Z Probe repeatability.
M73 - Set current print progress percentage for LCD.
M75 - Start the print job timer.
M76 - Pause the print job timer.
M77 - Stop the print job timer.
M78 - Print statistics about print jobs.
M80 - Turn on the power supply
M81 - Turn off the power supply.
M82 - Set E to absolute positioning.
M83 - Set E to relative positioning.
M85 - Set the inactivity timeout.
M92 - Set the number of steps-per-mm, per-inch, or per-degree
M100 - Observe memory used by code
M104 - Set a new target hot end temperature.
M105 - Send a temperature report to the host.
M106 - Turn on the fan and set its speed
M107 - Turn off a fan
M108 - Break out of the current waiting loop
M109 - Wait for the hot end to reach its target.
M110 - Set the current line number.
M111 - Report and optionally set the debug flags.
M112 - Shut everything down and halt the machine.
M113 - Get or set the host keepalive interval.
M114 - Report the current tool position to the host.
M115 - Print the firmware info and capabilities.
M117 - Set the message line on the LCD.
M118 - Send text to serial
M119 - Report endstop and probe states to the host.
M120 - Enable endstops and keep them enabled when not homing.
M121 - Disable endstops and keep them enabled when not homing.
M122 - Get TMC Debug Info
M123 - Report fan speeds from tachometers
M125 - Save current position and move to filament change position.
M126 - Open the valve for Baricuda 1.
M127 - Close the valve for Baricuda 1.
M128 - Open the valve for Baricuda 2.
M129 - Close the valve for Baricuda 2.
M140 - Set a new target bed temperature.
M141 - Set a new target chamber temperature.
M143 - Set a new target laser coolant temperature.
M145 - Set material presets in the LCD menu.
M149 - Set temperature units to Celsius, Fahrenheit, or Kelvin.
M150 - Set the color of the RGB(W) LED, backlight, or LED strip.
M154 - Periodically auto-report position to serial
M155 - Auto-report temperatures to host periodically.
M163 - Set a single mix factor for a mixing extruder.
M164 - Save the current mix as a virtual tool.
M165 - Set all mix factors for the mixing extruder.
M166 - Set a Gradient Mix
M190 - Wait for the bed to reach target temperature.
M191 - Wait for the chamber to reach target temperature.
M192 - Wait for the probe temperature sensor to reach a target
M193 - Set a new target laser coolant temperature.
M200 - Set the diameter for volumetric extrusion.
M201 - Set acceleration and frequency limits for print moves.
M203 - Set maximum feedrate for one or more axes.
M204 - Set the starting acceleration for moves by type.
M205 - Set some advanced settings related to movement.
M206 - Apply a persistent offset
M207 - Set options for firmware-based retraction.
M208 - Firmware-retraction recover settings.
M209 - Enable / disable auto-retraction.
M211 - Set and/or get the software endstops state
M217 - Set length and speed for filament swapping
M218 - Set the offset of a hotend (from hotend 0).
M220 - Set the global feedrate percentage.
M221 - Set the flow percentage, which applies to all E moves.
M226 - Wait for a pin to have a given state.
M240 - Trigger a camera shutter
M250 - Set and/or get the LCD contrast.
M256 - Set and/or get the LCD brightness.
M260 - Send data to the I2C bus.
M261 - Request and echo bytes from the I2C bus.
M280 - Set or get a servo position.
M281 - Set servo deploy and/or stow angles
M282 - Detach a servo until its next move
M290 - Babystep one or more axes
M300 - Play a single tone, buzz, or beep.
M301 - Set PID values for a hotend.
M302 - Set minimum extrusion temperature, allow cold extrusion.
M303 - Auto-tune the PID system to find stable values.
M304 - Set PID values for the heated bed.
M305 - Set (or report) custom thermistor parameters
M306 - Set MPC values for a hotend.
M350 - Set micro-stepping for drivers that support it
M351 - Directly set the micro-stepping pins
M355 - Turn the case light on or off, set brightness
M360 - Move to Theta A
M361 - Move to Theta-B
M362 - Move to Psi-A
M363 - Move to Psi-B
M364 - Move to Psi-C
M380 - Activate
M381 - Deactivate all extruder solenoids
M400 - Wait for all moves to finish
M401 - Deploy the bed probe
M402 - Stow the bed probe
M403 - Set filament type for Multi-Material Unit 2.0
M404 - Set the nominal diameter for filament width sensor auto-flow
M405 - Enable filament width sensor flow control
M406 - Disable filament width sensor flow control
M407 - Report the measured filament width
M410 - Stop all steppers instantly
M412 - Get/set filament runout detection parameters
M413 - Enable / disable power-loss recovery
M420 - Get and/or set bed leveling state and parameters
M421 - Set a single mesh Z height
M422 - Set a Z motor position for G34 Auto-Alignment
M423 - Modify, reset, and report X-Axis Twist Compensation data
M425 - Enable and tune backlash compensation
M428 - Set home offsets based on current position
M430 - Read and display current (A), voltage (V), and power (W)
M486 - Identify and cancel objects
M500 - Save settings to EEPROM.
M501 - Restore settings from EEPROM.
M502 - Restore all settings to factory defaults.
M503 - Report all settings that may be saved to EEPROM.
M504 - Validate the contents of the EEPROM.
M510 - Lock the machine if it has a passcode
M511 - Unlock the machine if it has a passcode
M512 - Set a numeric passcode for locking the machine
M524 - Abort an SD print started with [`M24`](/docs/gcode/M024.html)
M540 - Abort SD printing when an endstop is triggered.
M569 - Toggle stealthChop
M575 - Change the serial baud rate
M600 - Automatically change filament
M603 - Configure automatic filament change parameters
M605 - Set the behavior mode for a multiple nozzle setup
M665 - Set SCARA geometry values
M665 - Set delta geometry values
M666 - Set dual endstop offsets
M666 - Set Delta endstop adjustments
M672 - Set Duet Smart Effector sensitivity
M701 - Load filament
M702 - Unload filament
M710 - Set or report controller fan settings
M7219 - Control Max7219 Segmented LEDs
M808 - Set or go to a marker for looping G-code
M810-M819 - Set/execute one of ten G-code macros
M851 - Set the Z probe XYZ offset from nozzle
M852 - Misalignment in the XYZ axes.
M860-M869 - I2C position encoders for closed loop control
M871 - Configure probe temperature compensation
M876 - Handle Host prompt responses
M900 - Get and set Linear Advance K value
M906 - Set the motor current (in milliamps)
M907 - Set motor current via digital trimpot
M908 - Set a digital trimpot directly
M909 - Report DAC current values to host
M910 - Commit digipot/DAC value to external EEPROM
M911 - Driver overtemperature pre-warn condition
M912 - Clear overtemperature pre-warn condition flag
M913 - TMC driver switching to spreadCycle
M914 - Set sensorless homing sensitivity
M915 - Align ends of the Z axis and test torque
M916 - Find L6474 drive level (KVAL_HOLD) threshold
M917 - Find L6474 minimum current thresholds
M918 - Find L6474 speed threshold
M919 - Set Chopper Timing values for TMC stepper drivers
M928 - Log serial input to an SD file
M951 - Set / report Magnetic Parking Extruder settings
M993-M994 - load or back up SPI Flash and SD
M995 - Touch screen calibration for TFT display
M997 - Perform in-application firmware update
M999 - Return the machine to Running state
T0-T6 - Switch to the specified tool
 */

// Returns 'true' if the string is an INTEGER.
bool GCodeParser::isInteger(char* str)
{
    int i, len = strlen(str);
 
    if (len == 0)
        return (false);
    for (i = 0; i < len; i++) {
        if (str[i] != '0' && 
            str[i] != '1' && 
            str[i] != '2' && 
            str[i] != '3' && 
            str[i] != '4' && 
            str[i] != '5' && 
            str[i] != '6' && 
            str[i] != '7' && 
            str[i] != '8' && 
            str[i] != '9' || (str[i] == '-' && i > 0))
            return (false);
    }
    return (true);
}
 
// Returns 'true' if the string is a REAL NUMBER.
bool GCodeParser::isRealNumber(char* str)
{
    int i, len = strlen(str);
    bool hasDecimal = false;
 
    if (len == 0)
        return (false);
    for (i = 0; i < len; i++) {
        if (str[i] != '0' && 
            str[i] != '1' && 
            str[i] != '2' && 
            str[i] != '3' && 
            str[i] != '4' && 
            str[i] != '5' && 
            str[i] != '6' && 
            str[i] != '7' && 
            str[i] != '8' && 
            str[i] != '9' && 
            str[i] != '.' || (str[i] == '-' && i > 0))
            return (false);
        if (str[i] == '.')
            hasDecimal = true;
    }
    return (hasDecimal);
}
 
// Extracts the SUBSTRING.
char* GCodeParser::subString(char* str, int left, int right)
{
    int i;
    char* subStr = (char*)malloc(
                  sizeof(char) * (right - left + 2));
 
    for (i = left; i <= right; i++)
        subStr[i - left] = str[i];
    subStr[right - left + 1] = '\0';
    return (subStr);
}

/// <summary>
/// Remove or processes the comments. 
/// There are four types of comments
/// First type:
/// At any point, any text between partentheses. 
///    This comments will be removed before parse the block.
///    Comments can appear between words: S100 (set speed) F200 (feed) 
/// At the start of the block when this begins with ; (semicolon) or / (slash)
///    This second and third type are going be removed too, and the line is returned as command "IgnoreIt".
/// At the end of the line, after the ; (semicolon) that means end fo the block.
///    This comments will be deleted and the block parsed as conveniant.
/// When a line begins and ends with the same parentheses, the text inside it are considered an operator message
///    This comments is returned as line processed and a instruction tho show this message to the operator
/// </summary>
void GCodeParser::ProcessComments()
{
   //Serial.println("Processing Comments");

   char Comments[MAX_LINE_SIZE + 2];
   Comment[0] = '\0';
   int CommentIndex = 0;

   bool openParenthesisFound     = false;
   bool semicolonFound           = false;
	char c = '\0';

   // Remove spaces and tabs at the end.
   for (int x = strlen(line); line[x-1] == ' ' || line[x-1] == '\t'; x--) {
      line[x-1] = '\0';
   }

   // Remove spaces and tabs at the begining.
   while (line[0] == ' ' || line[0] == '\t') {
      int x = 0;
      while (line[x] != '\0') {
         line[x] = line[x + 1];
         x++;
      }
   };

   int LineLength = strlen(line);

   //Traces all the characters in the line
   int i = 0;
   while (line[i] != '\0') { 
      c = line[i]; //current character pointed

      // Look for start of comment.
      if (!semicolonFound && c == '(') {
         openParenthesisFound = true; // Open parenthesis... start of comment.
      }

      if (!openParenthesisFound && c == ';') {
         semicolonFound = true; // Semicolon... start of comment to end of line.
      }
   
      /*===============================================================================================*/
  
      // If we are inside a comment, we need to move it to the variable Comment to seperate it.
      if (openParenthesisFound || semicolonFound) {
         // Shift line left.
         for (int x = i; x <= LineLength; x++) {
            line[x] = line[x+1];
         }
         //Translate character to Comment
         Comment[CommentIndex] = c;
         Comment[CommentIndex+1] = '\0';
         CommentIndex++;
      }
      else
         i++;

      /*===============================================================================================*/

      // Look for end of comment.
      if (!semicolonFound && c == ')') {
         openParenthesisFound = false;

         // Is this the end of the comment? Scan forward for second closing parenthesis, but no opening parenthesis first.
         int x = i;
         while (line[x] != '\0') {
            if (line[x] == '(')
               break;
            if (line[x] == ')') {
               openParenthesisFound = true;
               break;
            }
            x++;
         }
      }
   }

   // Set pointer to comments.
   //comments = line + strlen(line) + 1;
   //if (line[0] == ';') comments = 0;
   //if (line[0] == '/') comments = 0;
   //if (line[0] == '%') comments = 0;
   //lastComment = comments;

   // There are several 'active' comments which look like comments but cause some action, like
   // '(debug,..)' or '(print,..)'. If there are several comments on a line, only the last comment
   // will be interpreted according to these rules. For this reason there is a pointer to the last comment.
   /*i = 0;
   openParenthesisFound = false;

   while (comments[i] != '\0') {
   char c = comments[i];

   // Open parenthesis... start of comment.
   if (c == '(') {
      lastComment = comments + i;
         openParenthesisFound = true; 
      }

      // Semicolon... start of comment to end of line, the last comment.
      if (!openParenthesisFound && c == ';') {
         lastComment = comments + i;
         break;
      }

      // Look for end of comment.
      if (c == ')') {   
         openParenthesisFound = false;

         // Is this the end of the comment? Scan forward for second closing parenthesis, but no opening parenthesis first.
         int scanAheadPointer = i + 1;

         while (comments[scanAheadPointer] != '\0') {
            if (comments[scanAheadPointer] == '(')
               break;

            if (comments[scanAheadPointer] == ')') {
               openParenthesisFound = true;
               break;
            }

            scanAheadPointer++;
         }
      }
      i++;
   }*/

   //Check if the comment is an operator message; 
   if ((strlen(line) == 0) && (Comment[0] == '(')){
      OperatorMessage = true;
      RemoveCommentSeparators();
      int x = 0;
      for (x = 0; Comment[x] != '\0'; x++) {
         line[x] = Comment[x];
      } 
      line[x+1] = '\0';
   }

   // The optional block deleted character the slash '/' when placed first on a line can be used
   // by some user interfaces to skip lines of code when needed.
   // The '%' is used to demarcate the beginning (first line) and end (last line) of the program. It is optional if the file has an 'M2' or 'M30'. 
   AvoidBlock = (strlen(line) == 0) || (line[0] == '/') || (line[0] == ';') || (line[0] == '%') || (!IsWord(line[0]));
}

/// <summary>
/// Removes the comment seperators for comments and last comment along with any leading spaces.
/// </summary>
/// <remark>Once removed they cannot be replaced.</remark>
void GCodeParser::RemoveCommentSeparators()
{
   int commentsLength = strlen(Comment);

   int  i                        = 0;
   bool openParentheseFound      = false;
   int  correctCommentsPointerBy = 0;
   char c                        = '\0';

   while (Comment[i] != '\0') {
      c = Comment[i];

      // Look for start of comment.
      if (c == '(') {
         Comment[i] = ' ';
         openParentheseFound = true; // Open parenthese... start of comment.
      }

      if (!openParentheseFound && c == ';') {
         Comment[i] = ' ';
         break;
      }

      // Look for end of comment.
      if (c == ')') {
         openParentheseFound = false;

         // Is this the end of the comment? Scan forward for second closing parenthese, but no opening parenthese first.
         int x = i + 1;

         while (Comment[x] != '\0') {
            if (Comment[x] == '(')
               break;

            if (Comment[x] == ')') {
               openParentheseFound = true;
               break;
            }
            x++;
         }

         if (!openParentheseFound) {
            // Shift line left.
            for (int x = i; x < commentsLength; x++) {
               Comment[x] = Comment[x + 1];
            }
         }
         else
            i++;
      }
      else
         i++;
   }

   // Remove spaces and tabs at the end.
   for (int x = strlen(Comment); Comment[x-1] == ' ' || Comment[x-1] == '\t'; x--) {
      Comment[x-1] = '\0';
   }

   // Remove spaces and tabs at the begining.
   while (Comment[0] == ' ' || Comment[0] == '\t') {
      int x = 0;
      while (Comment[x] != '\0') {
         Comment[x] = Comment[x + 1];
         x++;
      }
   };

}

/// <summary>
/// Looks for a word in the line.
/// </summary>
/// <param name="c">The letter of the word to look for in the line.</param>
/// <returns>A pointer to where the word starts.  Points to \0 if the word was not found.</returns>
int GCodeParser::FindWord(char letter)
{
   int  i                   = 0;
   bool openParentheseFound = false;
   bool semicolonFound      = false;

   while (line[i] != '\0') {
      // Look for the word.
      if (letter == line[i]) {
         return i;
      }

      i++;
   }

   return i;
}

/// <summary>
/// Looks through the code block to determin if a word exist.
/// </summary>
/// <param name="letter">The letter of the GCode word.</param>
/// <returns>True if the word exist on the line.</returns>
bool GCodeParser::HasWord(char letter)
{
   int  i = 0;

   if (IsWord(letter)) {
      i = FindWord(letter);

      if (line[i] == '\0') {
         return false;
      }
   }

   return true;
}

char wordLetter[] = { 'A', 'B', 'C', 'D', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\0' };

/// <summary>
/// Determine if the letter provided represents a valid GCode word.
/// </summary>
/// <param name="letter">The letter to be tested.</param>
/// <returns>True if the letter represents a valid word.</returns>
/// <remark>
/// Words may begin with any of the letters shown in the following
/// Table. The table includes N, @, ^ and / for completeness, even 
/// though, line numbers, polar coordinates and the block delete  
/// character are not considered words. Several letters (I, J, K,
/// L, P, R) may have different meanings in different contexts.
/// Letters which refer to axis names are not valid on a machines
/// which do not have the corresponding axis.
/// 
/// A - A axis of machine.
/// B - B axis of machine.
/// C - C axis of machine.
/// D - Tool radius compensation number.
/// F - Feed rate.
/// G - General function(See table Modal Groups).
/// H - Tool length offset index.
/// I - X offset for arcsand G87 canned cycles.
/// J - Y offset for arcsand G87 canned cycles.
/// K - Z offset for arcsand G87 canned cycles. Spindle - Motion Ratio for G33 synchronized movements.
/// L - generic parameter word for G10, M66and others.
/// M - Miscellaneous function(See table Modal Groups).
/// N - Line number. Line numbers are not considered words.
/// P - Dwell time in canned cyclesand with G4. Key used with G10.
/// Q - Feed increment in G73, G83 canned cycles.
/// R - Arc radius or canned cycle plane.
/// S - Spindle speed.
/// T - Tool selection.
/// U - U axis of machine.
/// V - V axis of machine.
/// W - W axis of machine.
/// X - X axis of machine.
/// Y - Y axis of machine.
/// Z - Z axis of machine
/// @ - Polar coordinate for the distance. Polar coordinates are not considered words.
/// ^ - Polar coordinate for the angle. Polar coordinates are not considered words.
/// / - The block delete character causes the processor to skips the line and is not considered a word.
/// % - Indicated the beginning and end of a program and is not considered a word.
/// </remark>
bool GCodeParser::IsWord(char letter)
{	
   int i = 0;
   while (wordLetter[i] != '\0') {
      if (wordLetter[i] == letter) {
         return true;
      }
      i++;
   }

   return false;
}

/// <summary>
/// Determine if the line contains any GCode words.
/// </summary>
/// <returns>True if there are no words.</returns>
/// <remarks>Words are not validated.<remark>
bool GCodeParser::NoWords()
{
   if (line[0] == '\0' || AvoidBlock) {
      return true;
   }

   int i = 0;
   while (wordLetter[i] != '\0') {
      if (HasWord(wordLetter[i])) {
         return false;
      }
      i++;
   }
   return true;
}

/// <summary>
/// Gets the value following the word.
/// </summary>
/// <param name="letter">The letter of the word to look for in the line.</param>
/// <returns>The value following the letter for the word.</returns>
/// <remarks>
/// Currently the parser is not sophisticated enough to deal with parameters, 
/// Boolean operators, expressions, binary operators, functions and repeated items.
/// </remarks>
double GCodeParser::GetWordValue(char letter)
{
   int i = FindWord(letter);

   if (line[i] != '\0')
      return (double)strtod(&line[i + 1], NULL);

   return 0.0;
}
