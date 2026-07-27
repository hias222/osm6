#include <string>
#include <stdio.h>
#include <string.h>

#include "analyseData.h"
#include "serialUtils.h"
#include "helperFunctions.h"
#include "analyseStartStop.h"
#include "analyseHeader.h"
#include "analyseRunningTime.h"
#include "analyseLane.h"

// #define debug

// #define debug_lane_pointer

#define COLORADO_CHANNELS 32
#define COLORADO_ADDRESS_WORD_MASK 0x80
#define COLORADO_ROWS 16

#define DISPLAY_LANE_COUNT 10
#define BUFFER_LENGTH 24

#define MQTT_MESSAGE_LENGTH 16
// for the message to broker
#define MQTT_LONG_LENGTH 25

uint8_t osm6_start_detected;
uint8_t osm6_end_detected;
uint8_t buf[BUFFER_LENGTH];
uint8_t paket1[BUFFER_LENGTH];
uint8_t paket2[BUFFER_LENGTH];
uint8_t part1_exist;
uint8_t part2_exist;
uint8_t colorado_control_update;
uint8_t **colorado_data;

uint8_t colorado_control_channel;
uint8_t colorado_control_bit;
uint8_t in_count;
uint8_t colorado_digit_no;

#define SOH 0x01
#define STX 0x02
#define HOME 0x08
#define LF 0x0A
#define DC2 0x12
#define DC4 0x14
#define EOT 0x04

const uint8_t colorado_channel_length[COLORADO_CHANNELS] = {7, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 9, 0, 9, 9, 0, 9, 9, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int loop;

int initReadData()
{
    printf("serialconnect - Load timing data\n");

    int i, o, ncolumns = 2, nrows = 3;

    // only way to alloc 2 dimensonal

    colorado_data = (uint8_t **)malloc(sizeof(uint8_t *) * 32);
    for (i = 0; i < 32; i++)
    {
        colorado_data[i] = (uint8_t *)malloc(sizeof(uint8_t) * 16);
    }

    for (o = 0; o < 32; o++)
    {
        for (i = 0; i < 16; i++)
        {
            colorado_data[o][i] = 0x01;
        }
    }

    // more inits

    initanalyseData();
    initRunninTime();
    initanalyseLane(DISPLAY_LANE_COUNT, MQTT_MESSAGE_LENGTH);
    initanalyseHeader(MQTT_LONG_LENGTH);
#ifdef debug
    printf("analyseData - INIT reaady\n");
#endif
    return 0;
}

int cleanReadData()
{
    cleananalyseData();
    cleananalyseLane();
    free(colorado_data);
    return 0;
}

int putReadData(uint8_t ReadData)
{
    uint8_t j;

    if (ReadData == SOH)
    {
        // wir starten neu mit sammeln
        in_count = 1;
        osm6_start_detected = 0x01;
        osm6_end_detected = 0x00;
        buf[0] = ReadData;
        // colorado_control_bit = buf[0] & 0x01;
        // colorado_control_channel = (~(buf[0] >> 1)) & 0x1F;
    }
    else if (ReadData == EOT)
    {
        osm6_end_detected = 0x01;
        buf[in_count] = ReadData;

        if (buf[1] == STX && buf[2] == HOME && buf[3] == LF)
        {
            if (part1_exist == 1)
            {
                part2_exist = 1;
                memcpy(paket2, &buf[0], in_count);
            }

#ifdef debug
            printf("analyseData - Paket 2 \n");
#endif

        }
        else if (buf[1] == STX && buf[2] == HOME)
        {
            part1_exist = 1;
            part2_exist = 0;
            memcpy(paket1, &buf[0], in_count);

            getHeader(paket1);

#ifdef debug
            printf("analyseData - Paket 1 \n");
#endif
        }
        else
        {

            printf("todo - ping \n");

#ifdef debug
            printf("analyseData - Error \n");
#endif
        }

#ifdef debug
        if (part2_exist == 1)
        {
            printf("analyseData - Paket 1 + Paket 2 \n");
        }
#endif

        // wir haben genug und verarbeiten

        // please check number lanes in colorado config !!!!!!!!!
        // analyseActiveData(colorado_control_channel, &colorado_data[colorado_control_channel]);
        // checkStartStop(&colorado_data[colorado_control_channel]);

        // wir analsieren die zeit um unötiges schicken zu vermeiden
        // getTime(&colorado_data[colorado_control_channel]);

        //
        // getHeader(&colorado_data[colorado_control_channel]);

        // storeRounds(&colorado_data[colorado_control_channel]);
    }
    else
    {
        if (0x01 == osm6_start_detected)
        { // wir hatten ein Start erkannt => Daten speichern
            buf[in_count] = ReadData;
            in_count++;
            if (in_count > 24)
            { // Ups... Da ist was schief gelaufen. Mehr als 8 Bytes bis zum nächsten Adress Word
                osm6_start_detected = 0x00;
                osm6_end_detected = 0x00;
            }
        }
    }

    return true;
}
