#pragma once

// A 2D vertex with x and y coordinate
struct Vertex
{
    float x;
    float y;
};

// A singe pen stroke to a new position, with a flag for pen down status
struct PenStroke
{
    struct Vertex NewPosition;
    int PenDownStatus;
};

// A series of PenStrokes written from a LocalOrigin
struct PenStrokeSeries
{
    int StrokeCount;
    struct Vertex LocalOrigin;
    struct PenStroke* StrokeData;
};

// Struct holding all information from fontfile for a single character
struct CharData
{
    char characterEncoded;
    int StrokeCount;
    struct PenStroke* StrokeData;
};

struct GCodeGeneratorInput
{
    char* inputWord;
    float fontSize;
    struct Vertex origin;
};


int InitialiseRobot(char* buffer);
int ShutdownRobot();

int WordFitsOnPage(char* inputWord, float fontSize, struct Vertex Origin);

char* GenerateGCodeForWord(struct GCodeGeneratorInput* input, struct CharData FontData[]);
char* GenerateGCodeForLetter(struct PenStrokeSeries LetterData, float fontSize);

void SendCommands(char* buffer);