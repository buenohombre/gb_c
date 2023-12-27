#ifndef C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_SENSORDATA_H_
#define C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_SENSORDATA_H_

#include "stdint.h"

union SensorData
{ 
    struct
    {
        uint32_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        int8_t temperature;
    } Data;

    int8_t Error;
};

typedef union SensorData SensorData;

#endif // C__USERS_DMITRY_SOURCE_REPOS_HOMEWORK_C_13_SENSORDATA_H_