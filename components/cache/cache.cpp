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
* Cache.cpp
*
* Created: 10/15/2019 4:14:53 AM
* Author: juneh
*/

#include "cache.h"
#include "../int/int.h"

void Cache::boot()
{
  // Make sure cache is disabled
  enableCache(false);

  // Leverage all 4 ways
  // TCM doesn't benefit this application
  enabledWays(4);

  // Invalidate ways 0 & 1
  invalidateWay(0);
  invalidateWay(1);

  // Re-enable cache in a way that let's us load-n-lock
  cacheLoadNLockInit();

  // We're already at way 0, begin loading
  // The linker places a section at 0x3F800 that is 2KB in size (2 Ways in size) named "lnl"
  // Most all the code which runs as usual is placed there
  // These need to be locked into 2 ways, leaving 2 other ways for usual program activity
  // In other words all code in "lnl" will run with 0 wait states and perfect speed
  cacheLoad((volatile var8*)0x3F800, waySize);
  cacheLoad((volatile var8*)(0x3F800 + 0x400), waySize);

  // Lock the loaded code
  cacheLock(0);
  cacheLock(1);

  // Re-enable instruction caching
  cacheInstrEnable(true);
}

void Cache::enableCache(bool val)
{
  if(CMCC->SR.bit.CSTS == val)
    return;

  CMCC->CTRL.bit.CEN = val;
  while(CMCC->SR.bit.CSTS != val);
}

void Cache::enabledWays(var8 val)
{
  if(val == 1)
    CMCC->CFG.bit.CSIZESW = 0; // 1 KB (1 Way) (3KB TCM)
  else if(val == 2)
    CMCC->CFG.bit.CSIZESW = 1; // 2 KB (2 Ways) (2KB TCM)
  else if(val > 2)
    CMCC->CFG.bit.CSIZESW = 2; // 4 KB (4 Ways) (0KB TCM)
}

void Cache::cacheDataEnable(bool val)
{
  CMCC->CFG.bit.DCDIS = !val;
}

void Cache::cacheInstrEnable(bool val)
{
  CMCC->CFG.bit.ICDIS = !val;
}

void Cache::cacheLoadNLock(var8 wayIndex, volatile var8* src, var32 size)
{
  // Disable cache
  enableCache(false);

  // Invalidate by line the whole desired way
  invalidateWay(wayIndex);

  // Init cache for load-n-lock then seek to, load into, and lock way
  cacheLoadNLockInit();
  cacheSeekWay(wayIndex);
  cacheLoad(src, size);
  cacheLock(wayIndex);

  // Re-enable instruction caching
  cacheInstrEnable(true);
}

void Cache::invalidateAll()
{
  CMCC->MAINT0.bit.INVALL = true;
}

void Cache::monitorCycles()
{
  CMCC->MCFG.bit.MODE = CMCC_MCFG_MODE_CYCLE_COUNT_Val;
}

void Cache::monitorHitsInstr()
{
  CMCC->MCFG.bit.MODE = CMCC_MCFG_MODE_IHIT_COUNT_Val;
}

void Cache::monitorHitsData()
{
  CMCC->MCFG.bit.MODE = CMCC_MCFG_MODE_DHIT_COUNT_Val;
}

void Cache::monitorEnable(bool val)
{
  CMCC->MEN.bit.MENABLE = val;
}

void Cache::monitorReset()
{
  CMCC->MCTRL.bit.SWRST = true;
}

var32 Cache::monitorValue()
{
  return CMCC->MSR.bit.EVENT_CNT;
}

volatile var32 Cache::dummy = 0;
