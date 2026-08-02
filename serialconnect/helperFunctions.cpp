#include <string>
#include <stdio.h>
#include <string.h>
#include "helperFunctions.h"

// #define DISPLAY_ONLY_LINE 3

uint8_t checkDisplayBit(uint8_t displaybit)
{
    if (displaybit == 0x0F)
    {
        return 0x00;
    }
    return displaybit;
};

uint8_t getNullBit()
{
    return 0x00;
}

char checkCharValue(char value)
{
    char characterToSend = (value == 32 || value == 48) ? '0' : (char)value;
    return characterToSend;
};

void checkCharValueNoNull(char value1, char value2, char *returnString)
{
    char mydata[4];
    // kein führendes Leerzeichen, wenn der erste Wert 0 ist, dann nur den zweiten Wert zurückgeben
    if (value1 == 32 || value1 == 48)
    {
        snprintf(mydata, sizeof(mydata), "%c", value2);
    }
    else
    {
        snprintf(mydata, sizeof(mydata), "%c%c", value1, value2);
    }

    strcpy(returnString, mydata);
};

uint8_t checkBitValue(uint8_t data)
{ 
    // history empty
    /*
    if (data == 0x0F)
    {
        return 0x0f;
    }
    */

    // lane 10 - switch to decimal lt
    if (data > 10)
    {
        return 0x00;
    }

    return data;
};

void showDisplayInternal(uint8_t data[])
{
    char mydata[64];

#ifdef DISPLAY_ONLY_LINE
    if (checkBitValue(data[0]) == DISPLAY_ONLY_LINE)
    {
#endif

        snprintf(mydata, sizeof(mydata), "-> %d-%d-%d-%d-%d-%d-%d-%d", checkBitValue(data[0]), checkBitValue(data[2]), checkBitValue(data[4]),
                checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]),
                checkBitValue(data[12]), checkBitValue(data[14]));
        printf("%s\n", mydata);
#ifdef DISPLAY_ONLY_LINE
    }
#endif
}

void showDisplayLine(uint8_t *data[])
{
    showDisplayInternal(*data);
}
