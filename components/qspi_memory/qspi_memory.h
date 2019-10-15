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
 * qspi.h
 *
 * Created: 9/28/2019 5:39:15 PM
 *  Author: juneh
 */

/*
 * External NVM (QSPI) [2MB]
 *
 * ENVM = 32 Blocks (2MB)
 * 1 Block = 16 Sectors (64KB)
 * 1 Sector = 16 Pages (4KB)
 * 1 Page = 256 Bytes
*/

/*
 * QSPI Memory Mode is used only for memory reads
 * It's actually somewhat of a nightmare to use for anything else
 * due to poor documentation and code examples. And poor implementation.
 * It's immensely fragile and practically anything breaks it mysteriously.
 * I can't rely on it for anything but it's decent to use for reading
 * most of the time.
*/

/*
 * This has turned into a mammoth of a class
 * Seriously, just for an external memory device!
*/

#ifndef QSPI_H_
#define QSPI_H_

#include "../../common/types.h"

enum : var16 {
  QSPI_BUFFER_END = 0x100
};

class QspiMemory
{
public:
  static void __attribute__((cold)) boot();

  static void modeSPI();
  static void modeMemory();

  // Switch to a very slow 4MHz speed
  static void speedLow();

  // Switch to a higher 120MHz speed
  static void speedHigh();

  // Switch to a custom speed
  static void speedAt(var32 speed);

  // Enable high-Performance Mode
  static void cmdHpm();

  // Enable Write
  static void cmdWriteEnable();

  // Disable Write
  static void cmdWriteDisable();

  // Chip Erase
  static void cmdCE();

  // Enable Reset & Reset CMDs
  static void cmdResetEnable();
  static void cmdReset();

  // Load Read Instructions
  static void instrRead();
  static var8 readStatusLow();
  static var8 readStatusHigh();

  // Load Write instructions
  static void instrWriteStatus(var16 status);
  static void instrWritePage(var32 addr, var8* data, var32 len, bool autoTrans = true);

  // Load Erase instructions
  static void instrEraseSector(var32 addr, var8 block, var8 sector, bool autoTrans = true);
  static void instrEraseBlock(var32 addr, var8 block, bool autoTrans = true);

  // Initiate Instruction Change
  static void instrChange();

  // Poll Ready State
  // This should be called right after a non-read instruction
  static void pollReady();

  // Ensures the QE and HPF bits are set
  static void ensureProperConfig();

  // Manually Send Data
  static void tx(var8 data, var8 dummy = 0);
  static void execute();
  static void txStart();

  // Get Addresses to ENVM
  static constexpr var32 __attribute__((const)) getAddrBlock(var8 block)
  {
    return offset + (blockSize * block);
  }

  static constexpr var32 __attribute__((const)) getAddrSector(var8 block, var8 sector)
  {
    return getAddrBlock(block) + (sectorSize * sector);
  }

  static constexpr var32 __attribute__((const)) getAddrPage(var8 block, var8 sector, var8 page)
  {
    return getAddrSector(block, sector) + (pageSize * page);
  }

  static constexpr var32 __attribute__((const)) getAddrQWord(var8 block, var8 sector, var8 page, var8 qword)
  {
    return getAddrPage(block, sector, page) + (qwordSize * qword);
  }

  static constexpr var32 offset = 0x04000000;
  static constexpr var8 blocks = 32;
  static constexpr var8 sectors = 16;
  static constexpr var8 pages = 16;
  static constexpr var8 qwords = 16;
  static constexpr var8 qwordSize = 16;
  static constexpr var32 pageSize = qwordSize * qwords;
  static constexpr var32 sectorSize = pageSize * pages;
  static constexpr var32 blockSize = sectorSize * sectors;
  static constexpr var32 size = blockSize * blocks;

  static void txDummy(var8 count);

  static bool completion;

private:
  // Setup physical pins for QSPI
  static void __attribute__((cold)) _setupPinIn(var8 port, var8 pin);
  static void __attribute__((cold)) _setupPinOut(var8 port, var8 pin);

  // Read sync (Used for instruction changes and loading)
  static void _instrReadSync();

  // Dummy variable for read-sync
  static var32 dummy;
};

#endif
