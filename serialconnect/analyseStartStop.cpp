#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"
#include "analyseLane.h"

// #define debug

int noworking;
bool pending, running;

bool b_running, b_stopping;

bool checkStartSignal(uint8_t data[])
{

#ifdef debug
    char mydata[64];

    snprintf(mydata, sizeof(mydata), "3: %c 4: %c 5: %c 6:%c 7: %c", 
         checkCharValue(data[3]), 
         checkCharValue(data[4]), 
         checkCharValue(data[5]), 
         checkCharValue(data[6]), 
         checkCharValue(data[7]));
    printf("getHeaderData: %s\n", mydata);
#endif

    char myonlinetime[2];
    char mystart[2];

    snprintf(mystart, sizeof(mystart), "%c", checkCharValue(data[4]));
    snprintf(myonlinetime, sizeof(myonlinetime), "%c", checkCharValue(data[3]));

#ifdef debug
    printf("CheckStart %s %s\n", mystart, myonlinetime);
#endif

    if (strcmp(mystart, "S") != 0)
    {
        return false;
    }

    if (strcmp(myonlinetime, "2") != 0)
    {
        return false;
    }

    return true;
};

bool checkStopSignal(uint8_t data[])
{
    char myonlinetime[2];

    snprintf(myonlinetime, sizeof(myonlinetime), "%c", checkCharValue(data[3]));

#ifdef debug
    printf("CheckStop %s\n", myonlinetime);
#endif

    if (strcmp(myonlinetime, "1") != 0)
    {
        return false;
    }

    return true;
};

void checkStartStopInternal(uint8_t data[])
{
    b_running = checkStartSignal(data);
    
    if (b_running)
    {
        char mydata[MQTT_LONG_LENGTH];
        sprintf(mydata, "start");
        mqtt_send(mydata);
        printf("----> start\n");
    }

    b_stopping = checkStopSignal(data);

    if (b_stopping)
    {
        char mydata[MQTT_LONG_LENGTH];
        sprintf(mydata, "stop");
        mqtt_send(mydata);
        printf("---> stop\n");
    }
};

void checkStartStop(uint8_t data[])
{
    checkStartStopInternal(data);
}