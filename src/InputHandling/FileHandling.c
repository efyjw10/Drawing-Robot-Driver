#include "InputHandling/FileHandling.h"

#include <stdio.h>
#include <stdlib.h>

#define NewCharacterCode 999

FILE* LoadFileFromPath(char* filePath)
{
    FILE* pFile;
    pFile = fopen(filePath, "r");

    if (pFile == NULL)
    {
        Fatal("File failed to open: %s", filePath);
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
            FontData[CharactersProcessed].StrokeCount = CurrentInstruction[2];

            FontData[CharactersProcessed].StrokeData = calloc(CurrentInstruction[2], sizeof(struct PenStroke));
            if (FontData[CharactersProcessed].StrokeData == NULL)
            {
                Fatal("Ran out of memory!");
                return;
            }

            CharactersProcessed += 1;
            CurrentStrokeIndex = 0;
        
        } else {
    
            // Temporary aliases to make unpacking clearer
            struct CharData* CurrentCharacter = &FontData[CharactersProcessed - 1];
            struct PenStroke* CurrentPenStroke = &CurrentCharacter->StrokeData[CurrentStrokeIndex];

            CurrentPenStroke->NewPosition.x =   CurrentInstruction[0];
            CurrentPenStroke->NewPosition.y =   CurrentInstruction[1];
            CurrentPenStroke->PenDownStatus =     CurrentInstruction[2];

            CurrentStrokeIndex += 1;
        }
    }

    fclose(fontFile);

    return;
}

char* ReadWordFromInputFile(FILE* inputFile, char* inputWord)
{
    int test = fscanf(inputFile, "%s", inputWord);
    if (test == EOF)
    {
        Fatal("Failed to Read File!");
        return NULL;
    }
    return inputWord;
}

char GetNextCharacter(FILE* inputFile)
{
    return fgetc(inputFile);
}