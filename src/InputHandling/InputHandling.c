#include "InputHandling.h"

#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#define NewCharacterCode 999

#pragma region User Input
void ClearInputBuffer()
{
    // https://stackoverflow.com/questions/2187474/i-am-not-able-to-flush-stdin-how-can-i-flush-stdin-in-c
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

float AskUserForFontSize()
{
    float fontSize;
    while (fontSize < 4 || fontSize > 10)
    {
        printf("Please input a desired Font Size [mm]: ");
        scanf("%f", &fontSize);

        // Prevent infinite loops if user inputs chars by clearing stdin buffer
        ClearInputBuffer();

        if (fontSize < 4 || fontSize > 10)
        {
            Error("Invalid font size. Please select a value between 4 and 10mm.\n");
        }
    }

    return fontSize;
}
#pragma endregion

#pragma region File Input
FILE* LoadFileFromPath(char* filePath)
{
    FILE* pFile;
    pFile = fopen(filePath, "r");

    if (pFile == NULL)
    {
        Fatal("File failed to open: %s\n", filePath);
        return NULL;
    }
}

void LoadFontDataFromFile(FILE* fontFile, struct CharData FontData[])
{
    if (fontFile == NULL)
    {
        Fatal("FontFile invalid!");
        return;
    }

    int CharactersProcessed = 0;
    int CurrentStrokeIndex = 0;

    int CurrentInstruction[3];

    while (!feof(fontFile))
    {
        fscanf(fontFile, "%d %d %d", &CurrentInstruction[0], &CurrentInstruction[1], &CurrentInstruction[2]);

        // If new character, define character and size of chardata penstrokes
        if( CurrentInstruction[0] == NewCharacterCode)
        {
            FontData[CharactersProcessed].characterEncoded = CurrentInstruction[1];
            FontData[CharactersProcessed].CharacterStrokes.StrokeCount = CurrentInstruction[2];

            FontData[CharactersProcessed].CharacterStrokes.StrokeData = calloc(CurrentInstruction[2], sizeof(struct PenStroke));
            if (FontData[CharactersProcessed].CharacterStrokes.StrokeData == NULL)
            {
                Fatal("Ran out of memory!");
                FontData = NULL;
                break;
            }

            CharactersProcessed += 1;
            CurrentStrokeIndex = 0;
        
        } else {
    
            // Temporary aliases to make unpacking clearer
            struct CharData* CurrentCharacter = &FontData[CharactersProcessed - 1];
            struct PenStroke* CurrentPenStroke = &CurrentCharacter->CharacterStrokes.StrokeData[CurrentStrokeIndex];

            CurrentPenStroke->NewPosition.x =   CurrentInstruction[0];
            CurrentPenStroke->NewPosition.y =   CurrentInstruction[1];
            CurrentPenStroke->PenDownStatus =     CurrentInstruction[2];

            CurrentStrokeIndex += 1;
        }
    }

    fclose(fontFile);

    return;
}

void ReadWordFromInputFile(FILE* inputFile, char* inputWord)
{
    int test = fscanf(inputFile, "%s", inputWord);
    if (test == EOF)
    {
        Fatal("Failed to Read File!");
        inputWord = NULL;
    }
    return;
}

int GetNextCharacterCode(FILE* inputFile)
{
    return fgetc(inputFile);
}
#pragma endregion