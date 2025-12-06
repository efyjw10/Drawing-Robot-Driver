
#include "RobotControl.h"

#include "SerialOutput/serial.h"
#include "rs232/rs232.h"

#include "log/log.h"

// Initialise Robot and send all preliminary commands
int InitialiseRobot(char* buffer)
{
    if ( CanRS232PortBeOpened() == -1 ) { Fatal("Unable to open the COM port \n"); return -1; }

    PrintBuffer("\n");
    WaitForDollar();

    buffer = "G1 X0 Y0 F1000\nM3\nS0\n";
    SendCommands(buffer);

    return 0;
}

int ShutdownRobot()
{
    CloseRS232Port();
    return 0;
}

int WordFitsOnPage(char* inputWord, float fontSize, struct Vertex origin)
{
    float spaceNeeded = strlen(inputWord)*(fontSize);
    if (PageWidth - origin.x < spaceNeeded) { return 0; }
    return 1;
}

char* GenerateGCodeForWord(struct GCodeGeneratorInput* input, struct CharData FontData[])
{    
    struct PenStrokeSeries LetterData;
    LetterData.LocalOrigin = input->origin;
    char* inputWord = input->inputWord;
    
    
    for (int i = 0; i < strlen(input->inputWord); i++)
    {
        int FontDataIndex = (int)inputWord[i];
        LetterData.StrokeCount = FontData[FontDataIndex].StrokeCount;
        LetterData.StrokeData = FontData[FontDataIndex].StrokeData;
        int characterEncoded = FontData[FontDataIndex].characterEncoded;

        GenerateGCodeForLetter(LetterData, input->fontSize);

        // Set new letter origin to last position in previous letter

        struct Vertex NewPosition = LetterData.StrokeData[LetterData.StrokeCount - 1].NewPosition;

        NewPosition.x = NewPosition.x * input->fontSize / DefaultFontSize + LetterData.LocalOrigin.x;
        NewPosition.y = NewPosition.y * input->fontSize / DefaultFontSize + LetterData.LocalOrigin.y;
        

        LetterData.LocalOrigin = NewPosition;
    }

    // Add space
    LetterData.StrokeCount = FontData[32].StrokeCount;
    LetterData.StrokeData = FontData[32].StrokeData;
    GenerateGCodeForLetter(LetterData, input->fontSize);

    return "";
}

char* GenerateGCodeForLetter(struct PenStrokeSeries LetterData, float fontSize)
{
    int PreviousPenDownStatus = 0;

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

        char* DrawOrMove;

        if (LetterData.StrokeData[i].PenDownStatus == 1)    DrawOrMove = "1";
        else                                                DrawOrMove = "0";

        struct Vertex NewPosition = LetterData.StrokeData[i].NewPosition;

        // Scale and translate new position to correct coordinate
        
        NewPosition.x = NewPosition.x * fontSize / DefaultFontSize + LetterData.LocalOrigin.x;
        NewPosition.y = NewPosition.y * fontSize / DefaultFontSize + LetterData.LocalOrigin.y;

        char StrokeGCode[25];
        sprintf(StrokeGCode, "G%s X%.2f Y%.2f\n", DrawOrMove, NewPosition.x, NewPosition.y);

        SendCommands(StrokeGCode);


        PreviousPenDownStatus = LetterData.StrokeData[i].PenDownStatus;
    }

    return "";
}

// Send the data to the robot - note in 'PC' mode you need to hit space twice
// as the dummy 'WaitForReply' has a getch() within the function.
void SendCommands(char* buffer)
{
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    WaitForReply();
    //Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}