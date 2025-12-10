#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <windows.h>

#include "rs232/rs232.h"
#include "logger/logger.h"

#include "InputHandling/InputHandling.h"
#include "RobotControl/RobotControl.h"

#define bdrate 115200
#define EOL 10

int main()
{
    // --- Initialise logger --- //
    LogInit();

    Info("Drawing-Robot-Driver - A program to convert a text file to gcode commands sent over a serial connection to a drawing robot.\n");

    FILE* fontFile;
    FILE* inputFile;

    // --- Load Files into Memory --- //
    fontFile = LoadFileFromPath(fontFilePath);
    inputFile = LoadFileFromPath(inputFilePath);
    if (fontFile == NULL || inputFile == NULL) { return -1; }

    // --- Read FontFile and Generate Lookup Table --- //
    struct CharData FontData[CharacterSetSize];
    LoadFontDataFromFile(fontFile, FontData);
    if (FontData == NULL) { return -1; }

    // --- Initialise Robot --- //
    Trace("Initialising the Robot...\n");
    if (InitialiseRobot() == -1) { return -1; }

    // --- Ask user to input a font size --- //
    float fontSize = AskUserForFontSize();

    struct Vertex GlobalOrigin; GlobalOrigin.x = 0; GlobalOrigin.y = 0;
    struct Vertex WordOrigin; WordOrigin.x = 0; WordOrigin.y = -fontSize;

    // --- Read word from input file --- //
    
    // Maximum characters supported, any more would spill over page.
    char* inputWord = calloc(PageWidth/MinimumFontSize, sizeof(char));

    Info("Gcode will be written to %s\n", logPath);

    while (!feof(inputFile))
    {
        ReadWordFromInputFile(inputFile, inputWord);
        int nextCharInFile = GetNextCharacterCode(inputFile);

        // --- Check if word on its own fits on page --- //
        if (CheckWordFitsOnPage(inputWord, fontSize, GlobalOrigin) == -1) { Error("Word '%s' cannot fit on page!\n", inputWord); break; }

        // --- Check word will fit on remaining page width --- //
        if (CheckWordFitsOnPage(inputWord, fontSize, WordOrigin) == -1)
        {
            WordOrigin.x = 0;
            WordOrigin.y -= (fontSize + LineSpacing);
        }

        // --- Generate gcode for word --- //
        struct GCodeGeneratorInput input;
        input.fontSize = fontSize; input.inputWord = inputWord; input.origin = WordOrigin; input.fontData = FontData;

        if (GenerateGCodeForWord(&input) == -1) { continue; }

        // --- Check for new line --- //
        if (nextCharInFile == EOL)
        {
            WordOrigin.x = 0;
            WordOrigin.y -= (LineSpacing + fontSize);
        }
        else
        {
            // --- Move beginning of next word to end of previous word --- //
            WordOrigin.x += strlen(inputWord) * fontSize + fontSize;
        }
    }

    fclose(inputFile);
    free(inputWord);

    // --- Shut down Robot --- //
    Trace("Shutting down Robot...\n");
    ShutdownRobot();

    return 0;
}