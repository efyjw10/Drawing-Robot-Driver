#pragma once

#include "logger/logger.h"
#include "RobotControl/RobotControl.h"

#include <stdio.h>

#pragma region User Input
/* Asks user for a font size between 4 and 10mm. Repeats if user inputs a
 number outside this range or an invalid value (anything that contains a letter).
 @return Floating point number between 4 and 10.
 */
float AskUserForFontSize();
#pragma endregion

#pragma region File Input

/* Loads a file to read from a path specified. 
 @param filePath (char*) The path to a file.

 @return File pointer if successful, null pointer if failed. Fatal log message displays error.
 */
FILE* LoadFileFromPath(char* filePath);


/* Loads font data from file into an array of structs. Pass ascii code as index to receive PenStroke data.
 @param fontFile (FILE*) A pointer to the font file.
 @param FontData (FontData*) This pointer will contain the data read from the font file.
 
 @return void
 */
void LoadFontDataFromFile(FILE* fontFile, struct CharData FontData[]);

/* Reads single word from file.
 @param inputFile (FILE*) A pointer to the input file.
 @param inputWord (char*) Will be modified to contain the word read from the file.
 
 @return void
 */
void ReadWordFromInputFile(FILE* inputFile, char* inputWord);

/* Reads single character from file.
 @param inputFile (FILE*) A pointer to the input file.
 
 @return (int) ASCII character code
 */
int GetNextCharacterCode(FILE* inputFile);
#pragma endregion