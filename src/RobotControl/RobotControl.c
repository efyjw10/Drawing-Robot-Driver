
#include "RobotControl.h"

#include "SerialOutput/serial.h"
#include "rs232/rs232.h"

#include "logger/logger.h"

#define SpaceChar 32


int InitialiseRobot()
{
    if ( CanRS232PortBeOpened() == -1 ) { Fatal("Unable to open the COM port \n"); return -1; }

    PrintBuffer("\n");
    WaitForDollar();

    char* buffer = "G1 X0 Y0 F1000\nM3\nS0\n";
    SendCommands(buffer);

    return 0;
}

void SendCommands(char* buffer)
{
    PrintBuffer (&buffer[0]);
    WaitForReply();
}


void ShutdownRobot()
{
    // Move pen up from page and return to global origin
    PrintBuffer("S0\n");
    WaitForReply();
    PrintBuffer("G0 X0 Y0 F1000\n");
    WaitForReply();

    CloseRS232Port();

    Info("Robot successfully shut down. Press enter to exit.");

    // Pause window so user can see robot shut down correctly.
    getchar();
    return;
}

int CheckWordFitsOnPage(char* inputWord, float fontSize, struct Vertex origin)
{
    float spaceNeeded = strlen(inputWord)*(fontSize);
    if (PageWidth - origin.x < spaceNeeded) { return -1; }
    return 0;
}

int GenerateGCodeForWord(struct GCodeGeneratorInput* input)
{    
    struct PenStrokeSeries LetterData;
    LetterData.LocalOrigin = input->origin;
    char* inputWord = input->inputWord;
    
    
    for (int i = 0; i < strlen(input->inputWord); i++)
    {
        int LetterIndex = (int)inputWord[i];

        if (LetterIndex < 0 || LetterIndex > CharacterSetSize) { Error("Character '%c' not supported!\n"); return -1; }

        LetterData.StrokeCount = input->fontData[LetterIndex].CharacterStrokes.StrokeCount;
        LetterData.StrokeData = input->fontData[LetterIndex].CharacterStrokes.StrokeData;
        int characterEncoded = input->fontData[LetterIndex].characterEncoded;

        GenerateGCodeForLetter(LetterData, input->fontSize);

        // Set new letter origin to last position in previous letter

        struct Vertex NewPosition = LetterData.StrokeData[LetterData.StrokeCount - 1].NewPosition;

        NewPosition.x = NewPosition.x * input->fontSize / DefaultFontSize + LetterData.LocalOrigin.x;
        NewPosition.y = NewPosition.y * input->fontSize / DefaultFontSize + LetterData.LocalOrigin.y;
        

        LetterData.LocalOrigin = NewPosition;
    }

    // Add space
    LetterData.StrokeCount = input->fontData[SpaceChar].CharacterStrokes.StrokeCount;
    LetterData.StrokeData = input->fontData[SpaceChar].CharacterStrokes.StrokeData;
    GenerateGCodeForLetter(LetterData, input->fontSize);

    return 0;
}

int GenerateGCodeForLetter(struct PenStrokeSeries LetterData, float fontSize)
{
    int PreviousPenDownStatus = 0;
    char* StrokeGCode = calloc(50, sizeof(char));

    // Check if PenDownStatus is changed from previous command
    for (int i = 0; i < LetterData.StrokeCount; i++)
    {
        if (LetterData.StrokeData[i].PenDownStatus != PreviousPenDownStatus)
        {
            char* PenCommand;
            if (LetterData.StrokeData[i].PenDownStatus == 1)    {   PenCommand = "S1000\n";   }
            else                                                {   PenCommand = "S0\n";      }

            SendCommands(PenCommand);

        }


        // Convert int PenDownStatus to char of 0 or 1
        char zero = '0';
        char DrawOrMove = (char)(LetterData.StrokeData[i].PenDownStatus + (int)zero);

        struct Vertex NewPosition = LetterData.StrokeData[i].NewPosition;

        // Scale and translate new position to correct coordinate
        
        NewPosition.x = NewPosition.x * fontSize / DefaultFontSize + LetterData.LocalOrigin.x;
        NewPosition.y = NewPosition.y * fontSize / DefaultFontSize + LetterData.LocalOrigin.y;

        sprintf(StrokeGCode, "G%c X%.2f Y%.2f\n", DrawOrMove, NewPosition.x, NewPosition.y);
        SendCommands(StrokeGCode);

        PreviousPenDownStatus = LetterData.StrokeData[i].PenDownStatus;
    }

    free(StrokeGCode);

    return 0;
}