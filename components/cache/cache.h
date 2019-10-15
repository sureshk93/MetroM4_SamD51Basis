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
* Cache.h
*
* Created: 10/15/2019 4:14:53 AM
* Author: juneh
*/

/*
 * Cache is a big topic with many options and configurations
 * However it's also an extremely unknown topic. Almost no helpful docs exist for the
 * SAMD5x and SAME5x chips. Even other SAM chips have little to no help.
 * You typically have to venture far out of the uC family to find various docs that explain
 * more and hope they are relevant in same way to your uC.
*/

#ifndef __CACHE_H__
#define __CACHE_H__

#include "../../common/types.h"
#include "sam.h"

class Cache
{
public:
  static void boot();
  static void enableCache(bool val);

  // Valid options are 1, 2, or 4
  // A way potentially increases program efficiency by 25%
  // and can be locked to specific code
  // However a way subtracts from TCM
  static void enabledWays(var8 val);

  static void cacheDataEnable(bool val);
  static void cacheInstrEnable(bool val);

  // Load important code into a way and lock it
  // This is copied from with modification the Atmel Example for
  // (http://ww1.microchip.com/downloads/en/DeviceDoc/How-to-Achieve-Deterministic-Code-Performance-using-CortexM-Cache-Controller-DS90003186A.pdf)
  static void cacheLoadNLock(var8 wayIndex, volatile var8* src, var32 size);

  // This breaks the loadNLock up into seperate functions
  // They have to be inlined though! We can't have stack exchanges, pushing, popping, and stuff happening in
  // this sensitive operation. Also make sure you know what your doing.

  // Re-enable cache in a way that allows for load-n-lock
  static inline void __attribute__((always_inline)) cacheLoadNLockInit()
  {
    enableCache(false);

    // Enable only data cache
    // This keeps instruction caching from mucking around with what we're doing
    cacheInstrEnable(false);
    cacheDataEnable(true);

    // Re-enable cache
    // This means WAY0 is the active way
    enableCache(true);
  }

  // Seek to a specific way
  // If your going to start at way 0 then you can skip this
  static inline void __attribute__((always_inline)) cacheSeekWay(var8 wayIndex)
  {
    // Find desired way
    for(var32 index = 0; index < wayCount; index++)
    {
      // Not the correct way but still need to pass through
      if(wayIndex != index)
        for(var8 i = 0; i < wayLines; i++)
          // Even though this is a dummy load, the CPU is filling the current way index and line
          // with this data
          // In a way we're erasing the cache line-by-line until the correct way
          // is gotten to
          dummy = *(var8*)dummyWayLine;
      else
        // We've seeked to the correct way
        break;
    }
  }

  // Lock that way
  static inline void __attribute__((always_inline)) cacheLock(var8 wayIndex)
  {
    // Lock Way
    // The way will no longer be used for usual program operations and will instead be used only for the
    // loaded code. That's one less way that can be used for normal program operations.
    // Your overall program efficiency potentially degrades by 25% but that locked code will run
    // with 0-wait states, 100% efficiency.
    CMCC->LCKWAY.bit.LCKWAY |= (1 << wayIndex);
  }

  // Load code into the way
  static inline void __attribute__((always_inline)) cacheLoad(volatile var8* src, var32 size)
  {
    // Caching a line into the way line auto-caches the entire line from start
    // Therefore we only need to tell it to cache the start of each line
    // In other words size needs to represent lines, not total bytes
    size /= wayLineSize;

    // Load way with code
    for(var8 i = 0; i < size; i++) {
      // Even though this is a dummy load, the CPU is filling the current way index and line
      // with this data, the data we want
      dummy = *src;

      // The entire line is auto-loaded, skip ptr forward the rest of the line
      src += wayLineSize;
    }
  }

  // Invalidate an entire way
  static inline void __attribute__((always_inline)) invalidateWay(var8 wayIndex)
  {
    // Invalidate by line the whole desired way
    for(volatile var8 i = 0; i < wayLines; i++)
    {
      CMCC->MAINT1.reg = CMCC_MAINT1_WAY(wayIndex) | CMCC_MAINT1_INDEX(i);
    }
  }

  static void invalidateAll();

  static void monitorCycles();
  static void monitorHitsInstr();
  static void monitorHitsData();
  static void monitorEnable(bool val);
  static void monitorReset();
  static var32 monitorValue();

  static constexpr var8 wayCount = 4;
  static constexpr var32 waySize = 1024;
  static constexpr var8 wayLines = 64;
  static constexpr var32 wayLineSize = 16;

  static volatile var32 dummy;

  // Dummy way line to load
  static constexpr var8 dummyWayLine[wayLineSize] = {
    0xA5, 0x5A, 0xA5, 0x5A,
    0xA5, 0x5A, 0xA5, 0x5A,
    0xA5, 0x5A, 0xA5, 0x5A,
    0xA5, 0x5A, 0xA5, 0x5A
  };
};

#endif
