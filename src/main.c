#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <windows.h>

#include "rs232/rs232.h"
#include "log/log.h"

#include "InputHandling/FileHandling.h"
#include "InputHandling/InputHandling.h"

#include "RobotControl/RobotControl.h"

#define bdrate 115200
#define CharacterSetSize 128
#define LineSpacing 5

void ClearInputBuffer();

int main()
{
    Info("Drawing-Robot-Driver - A program to convert a text file to gcode commands sent to an arduino to a drawing robot.\n");

    char* buffer;

    FILE* fontFile;
    FILE* inputFile;

    // --- Load Files into Memory --- //
    fontFile = LoadFileFromPath("C:/Dev/C++/UoN/Drawing-Robot-Driver/src/SingleStrokeFont.txt");
    inputFile = LoadFileFromPath("C:/Dev/C++/UoN/Drawing-Robot-Driver/src/test.txt");
    if (fontFile == NULL || inputFile == NULL) { return -1; }

    // --- Read FontFile and Generate Lookup Table --- //
    struct CharData FontData[CharacterSetSize];
    LoadFontDataFromFile(fontFile, FontData);
    if (FontData == NULL) { return -1; }

    // --- Ask user to input a font size --- //
    float fontSize = AskUserForFontSize();
    
    // --- Initialise Robot --- //
    Trace("Initialising the Robot...\n");
    if (InitialiseRobot(buffer) == -1) { return -1; }

    struct Vertex GlobalOrigin; GlobalOrigin.x = 0; GlobalOrigin.y = 0;
    struct Vertex WordOrigin; WordOrigin.x = 0; WordOrigin.y = -fontSize;

        // --- Read word from input file --- //
    
    // Maximum characters supported, any more would spill over page.
    char inputWord[PageWidth/MinimumFontSize];
    while (!feof(inputFile))
    {
        ReadWordFromInputFile(inputFile, inputWord);

        // --- Check if word on its own fits on page --- //
        if (WordFitsOnPage(inputWord, fontSize, GlobalOrigin) != 1) { Fatal("Word cannot fit on page!"); return -1; }

        // --- Check word will fit on remaining page width --- //
        if (WordFitsOnPage(inputWord, fontSize, WordOrigin) != 1)
        {
            WordOrigin.x = 0;
            WordOrigin.y -= (fontSize + LineSpacing);
        }

        // --- Generate gcode for word --- //
        struct GCodeGeneratorInput input;
        input.fontSize = fontSize; input.inputWord = inputWord; input.origin = WordOrigin;
        buffer = GenerateGCodeForWord(&input, FontData);

        // --- Send Commands --- //
        SendCommands(buffer);

        // --- Move beginning of next word to end of previous word --- //
        WordOrigin.x += strlen(inputWord) * fontSize + fontSize;

        // --- Clear buffer for next word --- //
        buffer = "";
    }
    

    // --- Shut down Robot --- //
    Info("Shutting down Robot...\n");
    ShutdownRobot();

    return 0;
}