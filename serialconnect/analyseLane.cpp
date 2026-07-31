#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"
#include "analyseRunningTime.h"

// #define debug

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

void getLaneTime(uint8_t part1[], uint8_t part2[])
{
    char mydata[64];
    char mykind[2];
    bool array_match = true;

    snprintf(mykind, sizeof(mykind), "%c", checkCharValue(part1[4]));

    if (strcmp(mykind, "I") == 0)
    {
        snprintf(mydata, sizeof(mydata), "lane %c %c%c:%c%c,%c%c 0",
                 checkCharValue(part2[4]),
                 checkCharValue(part2[11]),
                 checkCharValue(part2[12]),
                 checkCharValue(part2[14]),
                 checkCharValue(part2[15]),
                 checkCharValue(part2[17]),
                 checkCharValue(part2[18]));

#ifdef debug
        printf("Intermidate %s -> %s \n", mydata, mykind);
#endif

        mqtt_send(mydata);
    }

    if (strcmp(mykind, "A") == 0)
    {
        snprintf(mydata, sizeof(mydata), "lane %c %c%c:%c%c,%c%c %c%c",
                 checkCharValue(part2[4]),
                 checkCharValue(part2[11]),
                 checkCharValue(part2[12]),
                 checkCharValue(part2[14]),
                 checkCharValue(part2[15]),
                 checkCharValue(part2[17]),
                 checkCharValue(part2[18]),
                 checkCharValueNoNull(part1[17]),
                 checkCharValue(part1[18]));
#ifdef debug
        printf("Endzeit %s -> %s \n", mydata, mykind);
#endif

        mqtt_send(mydata);
    }

    if (strcmp(mykind, "R") == 0)
    {
        snprintf(mydata, sizeof(mydata), "reaction %c %c%c,%c%c",
                 checkCharValue(part2[4]),
                 checkCharValue(part2[14]),
                 checkCharValue(part2[15]),
                 checkCharValue(part2[17]),
                 checkCharValue(part2[18]));
#ifdef debug
        printf("Reaction %s -> %s \n", mydata, mykind);
#endif

        mqtt_send(mydata);
    }
}

void analyseActiveData(uint8_t part1[], uint8_t part2[])
{
#ifdef debug
    printf("analyseLane - analyseActiveData - start\n");
#endif

    getLaneTime(part1, part2);

#ifdef debug
    printf("analyseLane - analyseActiveData - end\n");
#endif
}