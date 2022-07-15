/*
MIT License

Copyright (c) 2022 Juan Carlos Cilelruelo Gonzalo

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

#ifndef GCodeParser_h 
   #define GCodeParser_h

#include <string.h>

const int MAX_LINE_SIZE = 256; // Maximun GCode line size.

typedef struct TGParameter {
  char Param; // The Letter of the parameter
  char* Value; // The Value of the parameter
}; 

typedef struct TResultParsed {  
   bool  IsComment;     
   char* Comment;
   /*************/
   bool  IsCommand;
   char  Type;       
   int   Number;
   struct TGParameter* Parameters;
   /*************/
   bool   Param_X; // A coordinate on the X axis
   double Value_X; 
   /*************/
   bool   Param_Y; // A coordinate on the Y axis 
   double Value_Y; 
   /*************/
   bool   Param_Z; // A coordinate on the Z axis 
   double Value_Z; 
   /*************/
   bool   Param_B; // Bed temperature (otherwise 60°C) to use for the test print.
   double Value_B;
   /*************/
   bool   Param_C; // Continue with the closest point (otherwise, don’t)
   double Value_C;
   /*************/
   bool   Param_E; // The amount to extrude between the start point and end point
   double Value_E; 
   /*************/
   bool   Param_F; // The maximum rate of the move between the start and end point
   double Value_F; 
   /*************/
   bool   Param_H; // Hot end temperature (otherwise 205°C) to use for the test print.
   double Value_H;
   /*************/
   bool   Param_I; // An offset from the current X position to use as the arc center
   double Value_I; 
   /*************/
   bool   Param_J; // An offset from the current Y position to use as the arc center
   double Value_J; 
   /*************/
   bool   Param_K; // Keep heaters on when done
   double Value_K;
   /*************/
   bool   Param_P; // Specify complete circles. (Requires ARC_P_CIRCLES)
   double Value_P; 
   /*************/
   bool   Param_Q; // Offset from the Y end point to the second control point
   double Value_Q; 
   /*************/
   bool   Param_R; // A radius from the current XY position to use as the arc center
   double Value_R;
   /*************/
   bool   Param_S; // Set the Laser power for the move.
   double Value_S; 
   /*************/
   bool   Param_T; // Number of triangles in the zigzag pattern
   double Value_T;  

};      

/// <summary>
/// The GCodeParser library is a lightweight G-Code parser for the Arduino using only
/// a single character buffer to first collect a line of code (also called a 'block') 
/// from a serial or file input and then parse that line into a code block and comments.
/// 
/// The parser was originally designed for use with code for the SphereBot, an EggBot clone.
/// 
/// Limitations
/// Currently the parser is not sophisticated enough to deal with parameters, Boolean
/// operators, expressions, binary operators, functions and repeated items. However, 
/// this should not be an obstacle when building 2D/3D plotters, CNC, and projects with 
/// an Arduino controller.
///
/// References
/// The following are just a few sources of information on GCode.
/// https://www.autodesk.com/products/fusion-360/blog/cnc-programming-fundamentals-g-code/
/// http://www.machinekit.io/docs/gcode/overview/
/// https://www.reprap.org/wiki/G-code
/// https://howtomechatronics.com/tutorials/g-code-explained-list-of-most-important-g-code-commands/ 
/// </summary>
class GCodeParser
{
private:
   void ProcessComments();
   void RemoveCommentSeparators();

   char* GetToken(int &left, char* str);

   bool isDelimiter(char ch);
   bool IsGCodeCommand(char* str);
   int  GetGCodeNumber(char* GCommand);
   bool isOperator(char ch);
   bool validIdentifier(char* str);
   bool isKeyword(char* str);
   bool isInteger(char* str);
   bool isRealNumber(char* str);
   char* subString(char* str, int left, int right);
public:
   bool AvoidBlock;
   bool OperatorMessage;
   char line[MAX_LINE_SIZE + 2];
   char Comment[MAX_LINE_SIZE + 2];

   TResultParsed resultParsed;  

   GCodeParser();
   void Initialize();
   void ParseLine(char* gCode);

   int  FindWord(char letter);
   bool HasWord(char letter);
   bool IsWord(char letter);
   bool NoWords();

   double GetWordValue(char letter);
};

#endif