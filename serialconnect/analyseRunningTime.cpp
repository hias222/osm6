#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"

#include <time.h>

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

#ifdef debug
    printf("Analyse Time End %02d:%02d:%02d Uhr\n",
           zeitstempel->tm_hour,
           zeitstempel->tm_min,
           zeitstempel->tm_sec);
#endif

    if (zeitstempel == NULL)
        return;

    // 1. Aktuelle Systemzeit holen
    time_t aktuelleZeit_t = time(NULL);

    // 2. Gespeicherte Zeit in time_t umwandeln
    struct tm tempGespeichert = *zeitstempel;
    time_t gespeicherteZeit_t = mktime(&tempGespeichert);

    if (gespeicherteZeit_t == (time_t)-1)
    {
        printf("Fehler bei der Zeitumwandlung!\n");
        return;
    }

    // 3. Differenz berechnen (Ergebnis ist ein double)
    double differenzInSekunden = difftime(aktuelleZeit_t, gespeicherteZeit_t);

    // In Ganzzahl umwandeln (negative Werte abfangen)
    long gesamtSekunden = (long)differenzInSekunden;
    if (gesamtSekunden < 0)
        gesamtSekunden = 0;

    // 3. Mathematische Aufteilung in Stunden, Minuten und Sekunden
    long stunden = gesamtSekunden / 3600;
    long minuten = (gesamtSekunden % 3600) / 60;
    long sekunden = gesamtSekunden % 60;

    // 4. Formatierte Ausgabe mit printf
    // %02ld sorgt für führende Nullen bei 2-stelligen Ganzzahlen (z.B. 05 statt 5)
    printf("Vergangene Zeit: %02ld:%02ld:%02ld\n", stunden, minuten, sekunden);

    char mydata[64];
    snprintf(mydata, sizeof(mydata), "time %02ld:%02ld,0", minuten, sekunden);
    
#ifdef debug
    printf("Debug: %s\n", mydata);
#endif
    
    mqtt_send(mydata);
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
    getTimeInternal(zeitstempel);

#ifdef debug
    printf("Stored Time End %02d:%02d:%02d Uhr\n",
           zeitstempel->tm_hour,
           zeitstempel->tm_min,
           zeitstempel->tm_sec);
#endif
};