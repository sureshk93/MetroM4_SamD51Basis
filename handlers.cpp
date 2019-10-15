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
 * interrupts.cpp
 *
 * Created: 10/5/2019 2:44:45 AM
 *  Author: juneh
 */

#include "sam.h"
#include "./common/types.h"
#include "./devices/display/DisplaySimple.h"
#include "./components/rt_clock/rt_clock.h"
#include "./devices/alarm/alarm.h"
#include "./components/power_manager/power_manager.h"

var16 countMS = 0;

// Called every 5ms
void __attribute__((hot,section(".lnl"))) onTimer5MS()
{
  // Send next digit every 5ms
  displaySimple.sendNext();
}

// Called every 1s
void __attribute__((hot)) onTimerS()
{
  // Do colon tick if enabled
  displaySimple.doTick();

  // Set time from RTC Clock if enabled
  displaySimple.doClockUpdate();
}

extern "C"
{
  // Ticks every 5ms
  void __attribute__((interrupt,hot,section(".lnl"))) TC0_Handler()
  {
    countMS += 5;
    onTimer5MS(); // Call every 5ms
    Alarm::alarmBtnSample();

    if(countMS >= 1000) {
      countMS = 0;
      onTimerS(); // Call every 1s
    }

    TC0->COUNT16.INTFLAG.bit.MC0 = true;
  }

  void __attribute__((interrupt,hot,section(".lnl"))) RTC_Handler()
  {
    Alarm::alarmFlagSet();
    RTC->MODE2.INTFLAG.bit.ALARM0 = true;
    RTC->MODE2.INTFLAG.bit.ALARM1 = true;
  }
};

void __attribute__((hot,section(".lnl"))) loop()
{
  // Keep in standby mode unless servicing an interrupt
  while(true) {
    PowerManager::enterStandby();
  }
}
