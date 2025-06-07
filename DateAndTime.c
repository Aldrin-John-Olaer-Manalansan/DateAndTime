/*
 * @File: DateAndTime.c
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

#include "DateAndTime.h"

const char* WEEKDAYNAMES[7] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

const char* MONTHNAMES[12] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

const uint8_t MONTHDAYS[12] = {
	31, /* Jan */
	28, /* Feb */
	31, /* Mar */
	30, /* Apr */
	31, /* May */
	30, /* Jun */
	31, /* Jul */
	31, /* Aug */
	30, /* Sep */
	31, /* Oct */
	30, /* Nov */
	31  /* Dec */
};

// Function to calculate the ISO week number
int8_t CalculateISOWeekNumber(int8_t month, int8_t day, int16_t year, int16_t *ISOYearNumber) {
    // Declare and initialize variables
    struct tm inputDate = {0};
    struct tm firstThursday = {0};
    int8_t weekNumber, dayOfWeek;

    // Populate the date structure for the input date
    inputDate.tm_year = year - 1900;
    inputDate.tm_mon = month - 1;  // Months are 0-based in struct tm
    inputDate.tm_mday = day;

    // Normalize the input date and calculate the day of the week
    mktime(&inputDate);
    dayOfWeek = (inputDate.tm_wday == 0) ? 7 : inputDate.tm_wday;  // Sunday is 7 in ISO

    // Adjust the date to the Thursday of the week containing the input date
    inputDate.tm_mday += 4 - dayOfWeek;  // n + daysToThursday: Thursday is ISO day 4
    mktime(&inputDate);

    // Determine the ISO year
    year = inputDate.tm_year + 1900;

    // Find January 4th of the ISO year (always in the first ISO week)
    firstThursday.tm_year = year - 1900;
    firstThursday.tm_mon = 0;  // January
    firstThursday.tm_mday = 4;
    mktime(&firstThursday);

    // Adjust to the nearest Thursday (ensure it's the correct ISO first week)
    int8_t firstThursdayDayOfWeek = (firstThursday.tm_wday == 0) ? 7 : firstThursday.tm_wday;
    firstThursday.tm_mday -= (firstThursdayDayOfWeek - 4);  // Align to Thursday
    mktime(&firstThursday);

    // Calculate the number of days since the first ISO week's Thursday
    int16_t daysSinceFirstThursday = (inputDate.tm_yday - firstThursday.tm_yday);

    // Calculate the ISO week number
    weekNumber = daysSinceFirstThursday / 7 + 1;

    // Handle cases where the week is part of the previous or next ISO year
    if (weekNumber < 1) {
        year--;
        firstThursday.tm_year--;
        firstThursday.tm_mday = 4;
        mktime(&firstThursday);
        firstThursdayDayOfWeek = (firstThursday.tm_wday == 0) ? 7 : firstThursday.tm_wday;
        firstThursday.tm_mday -= (firstThursdayDayOfWeek - 4);
        mktime(&firstThursday);
        daysSinceFirstThursday = (inputDate.tm_yday - firstThursday.tm_yday) + 365;
        weekNumber = daysSinceFirstThursday / 7 + 1;
    } else if (weekNumber > 52) {
        struct tm nextYearFirstThursday = {0};
        nextYearFirstThursday.tm_year = year - 1900 + 1;
        nextYearFirstThursday.tm_mon = 0;
        nextYearFirstThursday.tm_mday = 4;
        mktime(&nextYearFirstThursday);
        int nextYearFirstThursdayDayOfWeek = (nextYearFirstThursday.tm_wday == 0) ? 7 : nextYearFirstThursday.tm_wday;
        nextYearFirstThursday.tm_mday -= (nextYearFirstThursdayDayOfWeek - 4);
        mktime(&nextYearFirstThursday);

        if (inputDate.tm_yday >= nextYearFirstThursday.tm_yday) {
            year++;
            weekNumber = 1;
        }
    }

	if (ISOYearNumber != NULL) {
		*ISOYearNumber = year;
	}

	return weekNumber;
}

// Function to calculate the weekday(0-6 or in sunday to saturday format) from a date
int8_t DateToWeekDay(int8_t month, int8_t day, int16_t year) {
    // Adjust months and years for Zeller's Congruence
    if (month < 3) {
        month += 12;
        year--;
    }

    // Zeller's Congruence formula
    int k = year % 100;       // Year within the century
    int j = year / 100;       // Century
    int weekDay = (day + (13 * (month + 1)) / 5 + k + (k / 4) + (j / 4) - 2 * j) % 7;

    // Adjust result to match the desired range (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
    weekDay = (weekDay + 6) % 7;

    return weekDay;
}

bool isLeapYear(int16_t year) {
	if (year % 400 == 0) {
		return true;
	} else if (year % 100 == 0) {
		return false;
	} else if (year % 4 == 0) {
		return true;
	} else {
		return false;
	}
}

void MilliSecondsToTime(uint32_t milliSeconds, timeinfo_t* time) {
	time->hours = milliSeconds / (60U * 60U * 1000U);
	milliSeconds %= (60U * 60U * 1000U);
	time->minutes = milliSeconds / (60U * 1000U);
	milliSeconds %= (60U * 1000U);
	time->seconds = milliSeconds / 1000U;
	time->centiSeconds = (milliSeconds % 1000U) / 100U;
}
