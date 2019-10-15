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
 * boot.cpp
 *
 * Created: 9/26/2019 11:39:14 PM
 *  Author: juneh
 */

#include "boot.h"
#include "./components/clocks/clocks.h"
#include "./components/int/int.h"
#include "./components/qspi_memory/qspi_memory.h"
#include "./components/timers/timers.h"
#include "./components/power_manager/power_manager.h"
#include "./components/rt_clock/rt_clock.h"
#include "./components/supply/supply.h"
#include "./components/nvm/nvm.h"

#include "./devices/alarm/alarm.h"

void Boot::boot()
{
  Clocks::boot();
  Int::boot();
  NVM::boot();
  //QspiMemory::boot(); // Dropping QSPI completely for now, it's far too fragile and error-prone
  Timers::boot();
  PowerManager::boot();
  RTClock::boot();
  Supply::boot();

  Alarm::init();
}
