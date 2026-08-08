#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"

#define debug

// 100/s
//  -> 10s
#define INACTIVE_TIME_AFTER_START 200
#define SEND_TIME_EVERY_COUNTS 20

int hundredth;
int loopcount;

bool sendActiveState;
bool headerChange;

void startRunninTime()
{
    // hundredth = 0;
    loopcount = 0;
    sendActiveState = false;
    headerChange = false;
}

void stopRunningTime()
{
    // hundredth = 0;
    loopcount = 0;
    sendActiveState = false;
    headerChange = false;
}

int timehundredth(uint8_t data[])
{
    int minutes;
    minutes = checkBitValue(data[4]) * 10 + checkBitValue(data[6]);

    int seconds;
    seconds = checkBitValue(data[8]) * 10 + checkBitValue(data[10]);

    int decent;
    decent = checkBitValue(data[12]);
    int timehundredth = (minutes * 60 + seconds) * 100 + decent * 10;

    bool sendActiveState_save = sendActiveState;

    if (timehundredth > INACTIVE_TIME_AFTER_START)
    {
        sendActiveState = true;
    }

    if (timehundredth < INACTIVE_TIME_AFTER_START)
    {
        sendActiveState = false;
    }

    if (sendActiveState_save != sendActiveState && sendActiveState)
    {
        printf("Armed\n");
    }

    if (sendActiveState_save != sendActiveState && !sendActiveState)
    {
        printf("Paused\n");
    }

#ifdef debug
    printf("time 100 %d \n", timehundredth);
#endif

    return timehundredth;
}

void getTimeInternal(struct tm *zeitstempel)
{
    char mydata[64];
    // running = checknotnull(data);
    //  wir bauen bei laufwechsel das mit ein
    //  laufwechsel wird ausgeschaltet
    // hundredth = timehundredth(data);
    //

    // snprintf(mydata, sizeof(mydata), "time %d%d:%d%d,%d", checkBitValue(data[4]), checkBitValue(data[6]),
    //         checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]));
};

bool getsendActiveState()
{
    return sendActiveState;
}

void setsendActiveStateOff()
{
    sendActiveState = false;
    headerChange = true;

#ifdef debug
    if (headerChange)
    {
        printf("header change - paused\n");
    }
#endif
}

void sendPingTime(struct tm *zeitstempel)
{
#ifdef debug
    printf("Stored Time %02d:%02d:%02d Uhr\n",
           zeitstempel->tm_hour,
           zeitstempel->tm_min,
           zeitstempel->tm_sec);
#endif
    getTimeInternal(zeitstempel);

#ifdef debug
    printf("Stored Time End %02d:%02d:%02d Uhr\n",
           zeitstempel->tm_hour,
           zeitstempel->tm_min,
           zeitstempel->tm_sec);
#endif
};