#include <common.h>
#include <stddef.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

DLL_PUBLIC
size_t strftime(char* s, size_t maxsize, const char* format, const struct tm* timeptr) {
    const char* formatPos = format;
    char* strPos = s;
    size_t remainingSize = maxsize;
    
    while (*formatPos != '\0' && remainingSize > 1) {
        if (*formatPos == '%') {
            formatPos++;  // Move past the '%'

            if (*formatPos == '\0')
                break;  // Reached the end of format string

            switch (*formatPos) {
                case 'a':  // Abbreviated weekday name
                    if (timeptr->tm_wday >= 0 && timeptr->tm_wday <= 6) {
                        const char* weekdayAbbreviations[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
                        const char* abbreviation = weekdayAbbreviations[timeptr->tm_wday];
                        size_t copyLength = remainingSize < 4 ? remainingSize - 1 : 3;
                        strncpy(strPos, abbreviation, copyLength);
                        strPos += copyLength;
                        remainingSize -= copyLength;
                    }
                    break;

                case 'A':  // Full weekday name
                    if (timeptr->tm_wday >= 0 && timeptr->tm_wday <= 6) {
                        const char* weekdayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
                        const char* name = weekdayNames[timeptr->tm_wday];
                        size_t copyLength = remainingSize < 10 ? remainingSize - 1 : 9;
                        strncpy(strPos, name, copyLength);
                        strPos += copyLength;
                        remainingSize -= copyLength;
                    }
                    break;

                case 'b':  // Abbreviated month name
                    if (timeptr->tm_mon >= 0 && timeptr->tm_mon <= 11) {
                        const char* monthAbbreviations[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
                        const char* abbreviation = monthAbbreviations[timeptr->tm_mon];
                        size_t copyLength = remainingSize < 4 ? remainingSize - 1 : 3;
                        strncpy(strPos, abbreviation, copyLength);
                        strPos += copyLength;
                        remainingSize -= copyLength;
                    }
                    break;

                case 'B':  // Full month name
                    if (timeptr->tm_mon >= 0 && timeptr->tm_mon <= 11) {
                        const char* monthNames[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
                        const char* name = monthNames[timeptr->tm_mon];
                        size_t copyLength = remainingSize < 10 ? remainingSize - 1 : 9;
                        strncpy(strPos, name, copyLength);
                        strPos += copyLength;
                        remainingSize -= copyLength;
                    }
                    break;

                case 'c':  // Date and time representation
                    if (strftime(strPos, remainingSize, "%a %b %e %H:%M:%S %Y", timeptr) > 0) {
                        size_t copiedLength = strlen(strPos);
                        strPos += copiedLength;
                        remainingSize -= copiedLength;
                    }
                    break;

                case 'd':  // Day of the month (01-31)
                    snprintf(strPos, remainingSize, "%02d", timeptr->tm_mday);
                    strPos += 2;
                    remainingSize -= 2;
                    break;

                case 'H':  // Hour in 24-hour format (00-23)
                    snprintf(strPos, remainingSize, "%02d", timeptr->tm_hour);
                    strPos += 2;
                    remainingSize -= 2;
                    break;

                case 'I': // Hour in 12-hour format (01-12)
		    {
                    	int hour12 = timeptr->tm_hour % 12;
                    	if (hour12 == 0)
                    	    hour12 = 12;
                    	snprintf(strPos, remainingSize, "%02d", hour12);
                    	strPos += 2;
                    	remainingSize -= 2;
		    }
                    break;

                case 'j':  // Day of the year (001-366)
                    snprintf(strPos, remainingSize, "%03d", timeptr->tm_yday + 1);
                    strPos += 3;
                    remainingSize -= 3;
                    break;

                case 'm':  // Month (01-12)
                    snprintf(strPos, remainingSize, "%02d", timeptr->tm_mon + 1);
                    strPos += 2;
                    remainingSize -= 2;
                    break;

                case 'M':  // Minute (00-59)
                    snprintf(strPos, remainingSize, "%02d", timeptr->tm_min);
                    strPos += 2;
                    remainingSize -= 2;
                    break;

                case 'p':  // AM/PM designation
                    if (timeptr->tm_hour >= 0 && timeptr->tm_hour <= 11) {
                        strncpy(strPos, "AM", 2);
                        strPos += 2;
                        remainingSize -= 2;
                    } else {
                        strncpy(strPos, "PM", 2);
                        strPos += 2;
                        remainingSize -= 2;
                    }
                    break;

                case 'S':  // Second (00-59)
                    snprintf(strPos, remainingSize, "%02d", timeptr->tm_sec);
                    strPos += 2;
                    remainingSize -= 2;
                    break;

                case 'U':  // Week number of the year (Sunday as the first day) (00-53)
                    {
                        int weekNumber = (timeptr->tm_yday - timeptr->tm_wday + 7) / 7;
                        snprintf(strPos, remainingSize, "%02d", weekNumber);
                        strPos += 2;
                        remainingSize -= 2;
                    }
                    break;

                case 'w':  // Weekday as a decimal number (0-6, Sunday is 0)
                    snprintf(strPos, remainingSize, "%d", timeptr->tm_wday);
                    strPos += 1;
                    remainingSize -= 1;
                    break;

                case 'W':  // Week number of the year (Monday as the first day) (00-53)
                    {
                        int weekNumber = (timeptr->tm_yday - (timeptr->tm_wday ? (timeptr->tm_wday - 1) : 6) + 7) / 7;
                        snprintf(strPos, remainingSize, "%02d", weekNumber);
                        strPos += 2;
                        remainingSize -= 2;
                    }
                    break;

                case 'x':  // Date representation
                    if (strftime(strPos, remainingSize, "%m/%d/%y", timeptr) > 0) {
                        size_t copiedLength = strlen(strPos);
                        strPos += copiedLength;
                        remainingSize -= copiedLength;
                    }
                    break;

                case 'X':  // Time representation
                    if (strftime(strPos, remainingSize, "%H:%M:%S", timeptr) > 0) {
                        size_t copiedLength = strlen(strPos);
                        strPos += copiedLength;
                        remainingSize -= copiedLength;
                    }
                    break;

                case 'y':  // Year without century (00-99)
                    snprintf(strPos, remainingSize, "%02d", timeptr->tm_year % 100);
                    strPos += 2;
                    remainingSize -= 2;
                    break;

                case 'Y':  // Year with century
                    snprintf(strPos, remainingSize, "%04d", timeptr->tm_year + 1900);
                    strPos += 4;
                    remainingSize -= 4;
                    break;

                case 'Z':  // Timezone name or abbreviation
                    // Not supported in this simplified implementation
                    break;

                case '%':  // Literal '%'
                    *strPos++ = '%';
                    remainingSize--;
                    break;

                default:
                    // Invalid format specifier, skip it
                    break;
            }
        } else {
            *strPos++ = *formatPos;
            remainingSize--;
        }

        formatPos++;
    }

    *strPos = '\0';  // Null-terminate the resulting string
    return (size_t)(strPos - s);  // Return the length of the resulting string
}

DLL_PUBLIC
time_t mktime(struct tm *timeptr) {
    time_t result;

    // Normalize the tm structure (adjust overflow in fields)
    timeptr->tm_hour += timeptr->tm_min / 60;
    timeptr->tm_min %= 60;
    timeptr->tm_min += timeptr->tm_sec / 60;
    timeptr->tm_sec %= 60;
    timeptr->tm_hour %= 24;

    // Normalize the tm structure (adjust overflow in days)
    while (timeptr->tm_mon < 0) {
        timeptr->tm_year--;
        timeptr->tm_mon += 12;
    }
    while (timeptr->tm_mon >= 12) {
        timeptr->tm_year++;
        timeptr->tm_mon -= 12;
    }

    // Calculate days since the epoch (1970-01-01)
    result = (timeptr->tm_year - 70) * 365 + ((timeptr->tm_year - 69) / 4) - ((timeptr->tm_year - 1) / 100) + ((timeptr->tm_year + 299) / 400);
    result += (timeptr->tm_yday = 0);

    for (int i = 0; i < timeptr->tm_mon; ++i)
        result += timeptr->tm_mon * 306 + 5;
    result += timeptr->tm_mday - 1;

    result *= 24;
    result += timeptr->tm_hour;
    result *= 60;
    result += timeptr->tm_min;
    result *= 60;
    result += timeptr->tm_sec;

    // Adjust for local timezone offset
    if (timeptr->tm_isdst > 0)
        result -= 3600;
    else if (timeptr->tm_isdst < 0 && localtime(&result)->tm_isdst > 0)
        result += 3600;

    return result;
}
