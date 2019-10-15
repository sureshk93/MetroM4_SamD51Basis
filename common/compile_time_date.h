/*
 * compile_time_date.h
 *
 * Created: 10/7/2019 4:47:28 PM
 *  Author: juneh
 */

#ifndef COMPILE_TIME_DATE_H_
#define COMPILE_TIME_DATE_H_

#include "sam.h"
#include "./types.h"

// Converts a character to a number
constexpr var8 __attribute__((const, cold)) charToNum(var8 ch)
{
  if(ch == '0')
    return 0;
  else if(ch == '1')
    return 1;
  else if(ch == '2')
    return 2;
  else if(ch == '3')
    return 3;
  else if(ch == '4')
    return 4;
  else if(ch == '5')
    return 5;
  else if(ch == '6')
    return 6;
  else if(ch == '7')
    return 7;
  else if(ch == '8')
    return 8;
  else if(ch == '9')
    return 9;

  return 0;
}

// Gets the year at compilation minus reference leap year (2016)
constexpr var8 __attribute__((const, cold)) getCompileYear()
{
  return ((charToNum(__DATE__[7]) * 1000) + (charToNum(__DATE__[8]) * 100) + (charToNum(__DATE__[9]) * 10) + (charToNum(__DATE__[10]))) - 2016;
}

// Gets month at compilation
constexpr var8 __attribute__((const, cold)) getCompileMonth()
{
  var8 ret = 0;

  if(__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
    ret = 1;
  else if(__DATE__[0] == 'F')
    ret = 2;
  else if(__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
    ret = 3;
  else if(__DATE__[0] == 'A' && __DATE__[1] == 'p')
    ret = 4;
  else if(__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
    ret = 5;
  else if(__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
    ret = 6;
  else if(__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
    ret = 7;
  else if(__DATE__[0] == 'A' && __DATE__[1] == 'u')
    ret = 8;
  else if(__DATE__[0] == 'S')
    ret = 9;
  else if(__DATE__[0] == 'O')
    ret = 10;
  else if(__DATE__[0] == 'N')
    ret = 11;
  else if(__DATE__[0] == 'D')
    ret = 12;
  else ret = 0;

  return ret;
}

// Get Compilation Day
constexpr var8 __attribute__((const, cold)) getCompileDay()
{
  return ((charToNum(__DATE__[4]) * 10) + (charToNum(__DATE__[5])));
}

constexpr var8 __attribute__((const, cold)) getCompileHour()
{
  var8 ret = ((charToNum(__TIME__[0]) * 10) + (charToNum(__TIME__[1])));
  if(ret > 12)
    ret = (ret - 12) | RTC_MODE2_CLOCK_HOUR_PM_Val;

  return ret;
}

constexpr var8 __attribute__((const, cold)) getCompileMinute()
{
  return ((charToNum(__TIME__[3]) * 10) + (charToNum(__TIME__[4])));
}

constexpr var8 __attribute__((const, cold)) getCompileSecond()
{
  return ((charToNum(__TIME__[6]) * 10) + (charToNum(__TIME__[7])));
}

#endif
