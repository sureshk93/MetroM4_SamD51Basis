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
* NVM.h
*
* Created: 10/9/2019 1:28:18 AM
* Author: juneh
*/

/*
 * There are 3 non-volatile memories
 * 1) NVM (Internal Flash)
 * 2) SEEPROM (Internal Emulated EEPROM)
 * 3) ENVM (External Flash over QSPI)
 *
 * This class manages all the **internal** non-volatile memories. External NVM is managed by QSPIMemory
 * It appropriately breaks them down into banks, blocks, sectors, regions, pages, etc...
 * This is for correctly aligned navigation and usage. Correctly aligned data reads and writes
 * Lead to better results and usage along with a correct mind-frame on data positions.
*/

/*
 * NVM = 2 Banks (512KB) (352KB usable) (160KB reserved for SEEPROM)
 * 1 Bank = 32 blocks (256K) and 16 Regions
 * 1 Region = 2 Blocks (16KB)
 * 1 Block = 16 Pages (8KB)
 * 1 Page = 32 QWords (512B)
 * 1 QWord = 16 Bytes
 * 1 DWord = 8 Bytes
 *
 * DWords are used only for PBLDATA boundaries, Data must be transferred
 * from the same DWord to the page buffer before crossing a new DWord boundary
 * and starting over. The minimum write is still a QWord or a Page.
 *
 * Bank 1 is the Data/Auxillary Bank
 * ~63% of the blocks in Bank 1 are reserved for seeprom, ~38% are usable
 * Bank 1 only contains 12 blocks instead of the usual 32 blocks
 * Blocks 0-11 are available freely for use
 *
 * ======================================================================
 *
 * SEEPROM takes up 31% of usable NVM (160KB) and ~63% of usable NVM in the auxiliary bank (Bank 1)
 * SEEPROM = 144 Pages (Only 128 are usable) (64KB) [Why are we wasting space???]
 * 1 Page = 512 Bytes
 *
 * Internally SEEPROM reserves double it's size in NVM (128KB) plus margins (32KB or 16KB each half) [Why are there margins???]
 * this is for NVM wear leveling. Each NVM SEEPROM bank takes turns at 144 writes. [Presuming this number and reasoning!!!]
 *
 * !!!NVM SEEPROM has extremely vague and unclear details surrounding it, any information on it here
 * is a best guess and could be wrong!!!! (There's also too much unanswered questions)
 *
 * NVM SEEPROM = 2 Banks (160KB) and 144 Pages (72KB), only 128 pages are used (64K) [Why are we wasting pages???]
 * 1 Bank = 10 Blocks (80KB), 2 Blocks Unused (16KB), 8 Blocks Used (64KB) [Why are we wasting blocks and why don't the wasted pages and blocks add up???]
 * 1 Block = 16 Pages (8KB) [Presuming a block is the same size maybe???]
 * 1 Page = 32 QWords (512B)
 * 1 QWord = 16 Bytes
*/

#ifndef __NVM_H__
#define __NVM_H__

#include "sam.h"
#include "../../common/data.h"

class NVM
{
public:
  // Only needs to boot NVM and SEEPROM, ENVM was already booted by QSPI
  static void boot();

  // Internal NVM Memory
  static void nvmLowPower();
  static void nvmHighPower();
  static void nvmClearWriteBuffer();
  static void nvmSetSecurity();
  static void nvmBankSwap();
  static void nvmDisableCE();
  static void nvmEnableCE();
  static void nvmDisableBootProt();
  static void nvmEnableBootProt();

  static void nvmEraseBlock(var8 bank, var8 block);
  static void nvmLockRegion(var8 bank, var8 region);
  static void nvmUnlockRegion(var8 bank, var8 region);

  static var32 nvmStartWritePage(var8 bank, var8 block, var8 page);
  static var32 nvmStartWriteRegionPage(var8 bank, var8 region, var8 block, var8 page);
  static void nvmEndWritePage();

  static var32 nvmStartWriteQWord(var8 bank, var8 block, var8 page, var8 qword);
  static var32 nvmStartWriteRegionQWord(var8 bank, var8 region, var8 block, var8 page, var8 qword);
  static void nvmEndWriteQWord();

  // Internal SEEPROM Memory
  static void seepromLock();
  static void seepromUnlock();
  static void seepromLockReg();
  static void seepromUnlockReg();
  static void seepromRelocate();
  static void seepromFlush();
  static void seepromSectorChange(var8 sector);

  // Data Area

  static void dataAreaCheck();
  static void dataFormat();

  // Saves and restores all bytes in bkupram (The core programs memory state)
  // Useful for powering on and off keeping memory state
  static void dataSaveState();
  static void dataRestoreState();

  // Addressing
  // Returns a 32-bit requested address, this is not a pointer
  // The returned address needs to be made into a pointer of a certain size

  // Get Addresses to NVM
  static constexpr var32 __attribute__((const)) getAddrNvmBank(var8 bank)
  {
    return nvmOffset + (nvmBankSize * bank);
  }

  static constexpr var32 __attribute__((const)) getAddrNvmRegion(var8 bank, var8 region)
  {
    return getAddrNvmBank(bank) + (nvmRegionSize * region);
  }

  static constexpr var32 __attribute__((const)) getAddrNvmBlock(var8 bank, var8 block)
  {
    return getAddrNvmBank(bank) + (nvmBlockSize * block);
  }

  static constexpr var32 __attribute__((const)) getAddrNvmRegionBlock(var8 bank, var8 region, var8 block)
  {
    return getAddrNvmRegion(bank, region) + (nvmBlockSize * block);
  }

  static constexpr var32 __attribute__((const)) getAddrNvmPage(var8 bank, var8 block, var8 page)
  {
    return getAddrNvmBlock(bank, block) + (nvmPageSize * page);
  }

  static constexpr var32 __attribute__((const)) getAddrNvmQWord(var8 bank, var8 block, var8 page, var8 qword)
  {
    return getAddrNvmPage(bank, block, page) + (nvmQWordSize * qword);
  }

  static constexpr var32 __attribute__((const)) getAddrNvmDWord(var8 bank, var8 block, var8 page, var8 qword, var8 dword)
  {
    return getAddrNvmQWord(bank, block, page, qword) + (nvmDWordSize * dword);
  }

  static constexpr var32 __attribute__((const)) getAddrNvmRegionPage(var8 bank, var8 region, var8 block, var8 page)
  {
    return getAddrNvmRegionBlock(bank, region, block) + (nvmPageSize * page);
  }

  static constexpr var32 __attribute__((const)) getAddrNvmRegionQWord(var8 bank, var8 region, var8 block, var8 page, var8 qword)
  {
    return getAddrNvmRegionPage(bank, region, block, page) + (nvmQWordSize * qword);
  }

  static constexpr var32 __attribute__((const)) getAddrNvmRegionDWord(var8 bank, var8 region, var8 block, var8 page, var8 qword, var8 dword)
  {
    return getAddrNvmRegionQWord(bank, region, block, page, qword) + (nvmDWordSize * dword);
  }

  // Get Addresses to SEEPROM
  static constexpr var32 __attribute__((const)) getAddrSeepromBlock(var8 block)
  {
    return seepromOffset + (seepromBlockSize * block);
  }

  static constexpr var32 __attribute__((const)) getAddrSeepromPage(var8 block, var8 page)
  {
    return getAddrSeepromBlock(block) + (seepromPageSize * page);
  }

  static constexpr var32 __attribute__((const)) getAddrSeepromQWord(var8 block, var8 page, var8 qword)
  {
    return getAddrSeepromPage(block, page) + (seepromQWordSize * qword);
  }

  static constexpr var32 __attribute__((const)) getAddrSeepromDWord(var8 block, var8 page, var8 qword, var8 dword)
  {
    return getAddrSeepromQWord(block, page, qword) + (seepromDWordSize * dword);
  }

  // NVM, SEEPROM, and ENVM Stats
  static constexpr var32 nvmOffset = 0x00000000;
  static constexpr var8 nvmBanks = 2;
  static constexpr var8 nvmRegions = 16;
  static constexpr var8 nvmBlocks = 32;
  static constexpr var8 nvmRegionBlocks = 2;
  static constexpr var8 nvmPages = 16;
  static constexpr var8 nvmQWords = 32;
  static constexpr var8 nvmDWords = 2;
  static constexpr var32 nvmDWordSize = 8;
  static constexpr var32 nvmQWordSize = nvmDWordSize * nvmDWords;
  static constexpr var32 nvmPageSize = nvmQWordSize * nvmQWords;
  static constexpr var32 nvmBlockSize = nvmPageSize * nvmPages;
  static constexpr var32 nvmRegionSize = nvmBlockSize * nvmRegionBlocks;
  static constexpr var32 nvmBankSize = nvmBlockSize * nvmBlocks;
  static constexpr var32 nvmSize = nvmBankSize * nvmBanks;

  // (S)EEPROM technically doesn't have the concept of blocks and pages
  // It's all just one big byte-size reads, writes, and erases
  // However underneath it does use the the internal NVM so I like to still think of it
  // in terms of it's underlying units given they'll internally be used

  static constexpr var32 seepromOffset = 0x44000000;
  static constexpr var8 seepromBlocks = 8;
  static constexpr var8 seepromPages = 16;
  static constexpr var8 seepromQWords = 32;
  static constexpr var8 seepromDWords = 2;
  static constexpr var8 seepromDWordSize = 8;
  static constexpr var8 seepromQWordSize = seepromDWordSize * seepromDWords;
  static constexpr var32 seepromPageSize = seepromQWordSize * seepromQWords;
  static constexpr var32 seepromBlockSize = seepromPageSize * seepromPages;
  static constexpr var32 seepromSize = seepromBlockSize * seepromBlocks;

  // Axillary Data

  // Data Signature, this must be present at the start of axillary flash memory or the axillary memory is considered invalid and need
  // of formatting
  // CAFE BABE F1EE DECF FEED (10 bytes that must match)
  static constexpr var8 auxSigCompare[] = {
    0xCA, 0xFE, 0xBA, 0xBE, 0xF1, 0xEE, 0xDE, 0xCF, 0xFE, 0xED, // Signature (10 bytes)
    0x00,                                                       // Force Format Code (changing this causes a forced-format) (1 byte)
  };

  static constexpr var8 auxSignatureLen = 11;

  // Aux Blocks available to format
  static constexpr var8 auxNvmBlocks = 12;

  static var8 auxSig[auxSignatureLen];
};

#endif
