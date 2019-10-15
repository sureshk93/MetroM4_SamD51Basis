/*
 Copyright 2019 June Hanabi

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

/*
* RTClock.cpp
*
* Created: 10/7/2019 12:38:39 AM
* Author: juneh
*/

#include "rt_clock.h"
#include "sam.h"
#include "../../common/compile_time_date.h"

void RTClock::boot()
{
  // Enable clock mode and synchronized reading. Ensure RTC is clocked by a 1hz clock. Use AM/PM 12-hour time.
  RTC->MODE2.CTRLA.reg = RTC_MODE2_CTRLA_MODE_CLOCK | RTC_MODE2_CTRLA_CLOCKSYNC | RTC_MODE2_CTRLA_PRESCALER(0xB) |
                          RTC_MODE2_CTRLA_CLKREP;

  // Enable Alarm Interrupts
  RTC->MODE2.INTENSET.reg = RTC_MODE2_INTENSET_ALARM0 | RTC_MODE2_INTENSET_ALARM1;
  NVIC_EnableIRQ(RTC_IRQn);

  // Run in Debug Mode
  RTC->MODE2.DBGCTRL.bit.DBGRUN = true;

  // Set time
  RTC->MODE2.CLOCK.reg = RTC_MODE2_CLOCK_YEAR(getCompileYear()) | RTC_MODE2_CLOCK_MONTH(getCompileMonth()) | RTC_MODE2_CLOCK_DAY(getCompileDay()) |
                          RTC_MODE2_CLOCK_HOUR(getCompileHour()) | RTC_MODE2_CLOCK_MINUTE(getCompileMinute()) | RTC_MODE2_CLOCK_SECOND(getCompileSecond());
  while(RTC->MODE2.SYNCBUSY.bit.CLOCK);

  // Set Alarms
  RTC->MODE2.Mode2Alarm[0].ALARM.reg = RTC_MODE2_CLOCK_HOUR(9) | RTC_MODE2_CLOCK_MINUTE(00) | RTC_MODE2_CLOCK_SECOND(00);
  while(RTC->MODE2.SYNCBUSY.bit.ALARM0);

  RTC->MODE2.Mode2Alarm[1].ALARM.reg = RTC_MODE2_CLOCK_HOUR(9) | RTC_MODE2_CLOCK_MINUTE(00) | RTC_MODE2_CLOCK_SECOND(00) | RTC_MODE2_CLOCK_HOUR_PM;
  while(RTC->MODE2.SYNCBUSY.bit.ALARM1);

  // Set alarm on Hour, Minute, and Second match
  RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = 3;
  while(RTC->MODE2.SYNCBUSY.bit.MASK0);

  RTC->MODE2.Mode2Alarm[1].MASK.bit.SEL = 3;
  while(RTC->MODE2.SYNCBUSY.bit.MASK1);

  // Enable RTC Clock
  RTC->MODE2.CTRLA.bit.ENABLE = true;
  while(RTC->MODE2.SYNCBUSY.bit.ENABLE);
}

var8  __attribute__((section(".lnl"))) RTClock::getHour()
{
  var8 ret = RTC->MODE2.CLOCK.bit.HOUR & 0b1111;
  while(RTC->MODE2.SYNCBUSY.bit.CLOCKSYNC);
  ret = RTC->MODE2.CLOCK.bit.HOUR & 0b1111;

  return ret;
}

var8  __attribute__((section(".lnl"))) RTClock::getMinute()
{
  var8 ret = RTC->MODE2.CLOCK.bit.MINUTE;
  while(RTC->MODE2.SYNCBUSY.bit.CLOCKSYNC);
  ret = RTC->MODE2.CLOCK.bit.MINUTE;

  return ret;
}
