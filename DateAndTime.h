/*
 * @File: DateAndTime.h
 * @Author: Aldrin John O. Manalansan (ajom)
 * @Email: aldrinjohnolaermanalansan@gmail.com
 * @Brief: Library for calculating date and time 
 * @LastUpdate: June 7, 2025
 * 
 * Copyright (C) 2025  Aldrin John O. Manalansan  <aldrinjohnolaermanalansan@gmail.com>
 * 
 * This Source Code is served under Open-Source AJOM License
 * You should have received a copy of License_OS-AJOM
 * along with this source code. If not, see:
 * <https://raw.githubusercontent.com/Aldrin-John-Olaer-Manalansan/AJOM_License/refs/heads/main/LICENSE_AJOM-OS>
 */

#ifndef DateAndTime_H
#define DateAndTime_H

#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include "stdbool.h"
#include <time.h>

#define DATEANDTIME_NAMESLENGTH 3

#define TimeToMilliSeconds(hours, minutes, seconds, centiSeconds) (((((hours) * 3600) + ((minutes) * 60) + (seconds)) * 1000) + (centiSeconds * 100))

typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t centiSeconds;
} timeinfo_t;

extern const char* WEEKDAYNAMES[7];
extern const char* MONTHNAMES[12];
extern const uint8_t MONTHDAYS[12];

int8_t CalculateISOWeekNumber(int8_t month, int8_t day, int16_t year, int16_t *ISOYearNumber);
int8_t DateToWeekDay(int8_t month, int8_t day, int16_t year);
bool isLeapYear(int16_t year);
void MilliSecondsToTime(uint32_t milliSeconds, timeinfo_t* time);

#endif
