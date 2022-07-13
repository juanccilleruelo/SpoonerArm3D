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
   Initialize();

   int i = 0;
   while (Value[i] != '\0') {
      line[i] = Value[i];
      i++;
   }
  	
   line[i] = '\0';
   ProcessComments();
}

/*
   Parser functions 
*/

// Returns 'true' if the character is a DELIMITER.
bool GCodeParser::isDelimiter(char ch)
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}')
        return (true);
    return (false);
}
 
// Returns 'true' if the character is an OPERATOR.
bool GCodeParser::isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == '>' || ch == '<' ||
        ch == '=')
        return (true);
    return (false);
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
{
    if (!strcmp(str, "if"      ) || 
        !strcmp(str, "else"    ) ||
        !strcmp(str, "while"   ) || 
        !strcmp(str, "do"      ) ||
        !strcmp(str, "break"   ) ||
        !strcmp(str, "continue") || 
        !strcmp(str, "int"     ) || 
        !strcmp(str, "double"  ) || 
        !strcmp(str, "float"   ) || 
        !strcmp(str, "return"  ) || 
        !strcmp(str, "char"    ) || 
        !strcmp(str, "case"    ) || 
        !strcmp(str, "char"    ) || 
        !strcmp(str, "sizeof"  ) || 
        !strcmp(str, "long"    ) || 
        !strcmp(str, "short"   ) || 
        !strcmp(str, "typedef" ) || 
        !strcmp(str, "switch"  ) || 
        !strcmp(str, "unsigned") || 
        !strcmp(str, "void"    ) || 
        !strcmp(str, "static"  ) || 
        !strcmp(str, "struct"  ) || 
        !strcmp(str, "goto"    ))
        return (true);
    return (false);
}
 
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
 
// Parsing the input STRING.
void GCodeParser::parse(char* str)
{
   int left  = 0; 
   int right = 0;
   int len   = strlen(str);
 
   while (right <= len && left <= right) {
       if (isDelimiter(str[right]) == false)
           right++;

       if (isDelimiter(str[right]) == true && left == right) {
          if (isOperator(str[right]) == true)
              printf("'%c' IS AN OPERATOR\n", str[right]);

          right++;
          left = right;
       } 
       else if (isDelimiter(str[right]) == true && left != right || (right == len && left != right)) {
          char* subStr = subString(str, left, right - 1);

          if (isKeyword      (subStr) == true) printf("'%s' IS A KEYWORD\n"    , subStr); else 
          if (isInteger      (subStr) == true) printf("'%s' IS AN INTEGER\n"   , subStr); else 
          if (isRealNumber   (subStr) == true) printf("'%s' IS A REAL NUMBER\n", subStr); else 
          if (validIdentifier(subStr) == true && isDelimiter(str[right  - 1]) == false) printf("'%s' IS A VALID IDENTIFIER\n"    , subStr); else
          if (validIdentifier(subStr) == false && isDelimiter(str[right - 1]) == false) printf("'%s' IS NOT A VALID IDENTIFIER\n", subStr);
          left = right;
       }
   }
   return;
}

// DRIVER FUNCTION
/*int main()
{
     // maximum length of string is 100 here
    char str[100] = "int a = b + 1c; ";
 
    parse(str); // calling the parse function
 
    return (0);
}*/

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
   AvoidBlock = (strlen(line) == 0) || (line[0] == '/') || (line[0] == ';') || (line[0] == '%');
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

   //while (Comment[0] == ' ') {
   //   // Shift pointer right
   //   Comment[0] = Comment[1];
   //}

   //while (lastComment[0] == ' ') {
   //   // Shift pointer right  
   //   lastComment = lastComment + 1;
   //}
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
