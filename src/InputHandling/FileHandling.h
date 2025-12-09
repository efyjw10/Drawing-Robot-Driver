#pragma once

#include "log/log.h"

#include "RobotControl/RobotControl.h"

#include <stdio.h>

FILE* LoadFileFromPath(char* filePath);

void LoadFontDataFromFile(FILE* fontFile, struct CharData FontData[]);

char* ReadWordFromInputFile(FILE* inputFile, char* inputWord);

int GetNextCharacterCode(FILE* inputFile);