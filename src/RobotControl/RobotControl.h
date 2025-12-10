#pragma once

/* A single 2 dimensional vertex
@param x (float) The x co-ordinate of the vertex
@param y (float) The y co-ordinate of the vertex
*/
struct Vertex
{
    float x;
    float y;
};

/* A singe pen stroke to a new position, with a flag for pen down status
@param NewPosition (struct Vertex) The end of the pen movement 
@param PenDownStatus (int) Determines whether pen should be up or down during this movement
*/
struct PenStroke
{
    struct Vertex NewPosition;
    int PenDownStatus;
};

/* A series of Pen Stroke structs
@param StrokeCount (int) The number of strokes contained in this series
@param LocalOrigin (struct Vertex) The local origin all strokes are relative to
@param StrokeData (struct* PenStroke) Pointer to stroke data
*/
struct PenStrokeSeries
{
    int StrokeCount;
    struct Vertex LocalOrigin;
    struct PenStroke* StrokeData;
};

/* All data from font file needed to draw a character
@param characterEncoded (char) The character represented by the font data.
@param StrokeData (struct PenStrokeSeries) The series of pen strokes needed to write a character.
*/
struct CharData
{
    char characterEncoded;
    struct PenStrokeSeries CharacterStrokes;
};

/* All data from font file needed to generate the Gcode for a single word
@param inputWord (char*) The word to be converted to gcode
@param fontSize (float) The size of the font chosen by the user.
@param origin (struct Vertex) The position of the local origin in global space
@param fontData (struct CharData*) Pointer to the font data read from the font file.
*/
struct GCodeGeneratorInput
{
    char* inputWord;
    float fontSize;
    struct Vertex origin;
    struct CharData* fontData;
};

/* Initialises robot. Opens RS232 port and sends necessary commands to wake up and initialise the robot.

 @return 0 if successful, -1 if failed
 */
int InitialiseRobot();

/* Sends command to buffer

@param buffer (char*) String of Gcode commands to be sent to robot.

@returns void
*/
void SendCommands(char* buffer);

/* Shuts down robot. Moves pen up and back to global origin.

 @return 0
 */
void ShutdownRobot();

/* Checks if word can fit on page based on current position of end of previous word.

@param inputWord (char*) The word that needs to fit on the page.
@param fontSize (float) The size of the font the user selected.
@param Origin (struct Vertex) The position of the end of the previous word.

 @return 0 if word can fit, -1 if word cannot fit.
 */
int CheckWordFitsOnPage(char* inputWord, float fontSize, struct Vertex Origin);

/* Generates gcode for a word and sends commands to buffer.
@param input (struct GCodeGeneratorInput*) Pointer to input struct that contains all input information

@param 

 @return 0 if successful, -1 if failed.
 */
int GenerateGCodeForWord(struct GCodeGeneratorInput* input);

/* Generates gcode for a single letter and sends commands to buffer.
@param LetterData (struct PenStrokeSeries) All the data needed to draw a single character
@param fontSize (float) Font size inputted by user

 @return 0 if successful, -1 if failed.
 */
int GenerateGCodeForLetter(struct PenStrokeSeries LetterData, float fontSize);

