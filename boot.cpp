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
