#include "InputHandling.h"

#include <string.h>

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
        Info("Please input a desired Font Size [mm]: ");
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