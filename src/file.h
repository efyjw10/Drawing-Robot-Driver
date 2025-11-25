#pragma once

#include <stdio.h>
#include "log/log.h"

FILE OpenFile(char* filePath)
{
    FILE* pFile;
    pFile = fopen(filePath, "r");

    if (pFile == NULL)
    {
        Fatal("File failed to open: %s", filePath);
        exit();
    }
}