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
* NVM.cpp
*
* Created: 10/9/2019 1:28:17 AM
* Author: juneh
*/

#include "nvm.h"
#include "sam.h"

extern var32 _sbkupramsave, _ebkupramsave, _sbkupram;

void __attribute__((cold)) NVM::boot()
{
  // Enter and exist standby mode based on cpu standby state
  // If 2 jobs occur in the same bank, it's ok to pause the first one
  // Calculate for me the wait-states based on CPU Clock Cycle (Costs a bit more power)
  NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_PRM_FULLAUTO | NVMCTRL_CTRLA_SUSPEN | NVMCTRL_CTRLA_AUTOWS;
  NVMCTRL->DBGCTRL.reg = NVMCTRL_DBGCTRL_ECCELOG; // Log ECC Errors but still correct them
  NVMCTRL->SEECFG.reg = NVMCTRL_SEECFG_WMODE_BUFFERED; // Use Auto Page Reallocation and write manually

  // Check data area for validation, if invalid issue a format
  dataAreaCheck();
}

void NVM::nvmPollReady()
{
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmLowPower(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SPRM;

  if(autoWait)
    nvmPollReady();
}

void NVM::nvmHighPower(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_CPRM;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmClearWriteBuffer(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_PBC;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmSetSecurity(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SSB;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmBankSwap(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_BKSWRST;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmDisableCE(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_CELCK;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmEnableCE(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_CEULCK;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmDisableBootProt(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SBPDIS;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmEnableBootProt(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_CBPDIS;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmEraseBlock(var8 bank, var8 block, bool autoWait)
{
  NVMCTRL->ADDR.bit.ADDR = getAddrNvmBlock(bank, block);
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_EB;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmLockRegion(var8 bank, var8 region, bool autoWait)
{
  NVMCTRL->ADDR.bit.ADDR = getAddrNvmRegion(bank, region);
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_LR;
  if(autoWait)
    nvmPollReady();
}

void NVM::nvmUnlockRegion(var8 bank, var8 region, bool autoWait)
{
  NVMCTRL->ADDR.bit.ADDR = getAddrNvmRegion(bank, region);
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_UR;
  if(autoWait)
    nvmPollReady();
}

var32 NVM::nvmStartWritePage(var8 bank, var8 block, var8 page, bool autoWait)
{
  nvmClearWriteBuffer(autoWait);
  var32 addr = getAddrNvmPage(bank, block, page);
  NVMCTRL->ADDR.bit.ADDR = addr;
  return addr;
}

var32 NVM::nvmStartWriteRegionPage(var8 bank, var8 region, var8 block, var8 page, bool autoWait)
{
  nvmClearWriteBuffer(autoWait);
  var32 addr = getAddrNvmRegionPage(bank, region, block, page);
  NVMCTRL->ADDR.bit.ADDR = addr;
  return addr;
}

void NVM::nvmEndWritePage(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_WP;
  if(autoWait)
    nvmPollReady();
}

var32 NVM::nvmStartWriteQWord(var8 bank, var8 block, var8 page, var8 qword, bool autoWait)
{
  nvmClearWriteBuffer(autoWait);
  var32 addr = getAddrNvmQWord(bank, block, page, qword);
  NVMCTRL->ADDR.bit.ADDR = addr;
  return addr;
}

var32 NVM::nvmStartWriteRegionQWord(var8 bank, var8 region, var8 block, var8 page, var8 qword, bool autoWait)
{
  nvmClearWriteBuffer(autoWait);
  var32 addr = getAddrNvmRegionQWord(bank, region, block, page, qword);
  NVMCTRL->ADDR.bit.ADDR = addr;
  return addr;
}

void NVM::nvmEndWriteQWord(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_WQW;
  if(autoWait)
    nvmPollReady();
}

void NVM::seepromPollReady()
{
  nvmPollReady();
  while(NVMCTRL->SEESTAT.bit.BUSY);
}

void NVM::seepromLock(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_LSEE;
  if(autoWait)
    seepromPollReady();
}

void NVM::seepromUnlock(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_USEE;
  if(autoWait)
    seepromPollReady();
}

void NVM::seepromLockReg(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_LSEER;
  if(autoWait)
    seepromPollReady();
}

void NVM::seepromUnlockReg(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_USEER;
  if(autoWait)
    seepromPollReady();
}

void NVM::seepromRelocate(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SEERALOC;
  if(autoWait)
    seepromPollReady();
}

void NVM::seepromFlush(bool autoWait)
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SEEFLUSH;
  if(autoWait)
    seepromPollReady();
}

void NVM::seepromSectorChange(var8 sector, bool autoWait)
{
  if(sector == 0)
    NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_ASEES0;
  else
    NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_ASEES1;

  if(autoWait)
    seepromPollReady();
}

void NVM::dataAreaCheck(bool autoWait)
{
  bool pass = true; // Optimistic pass strategy
  for(var8 i = 0; i < auxSignatureLen; i++)
  {
    if(auxSig[i] != auxSigCompare[i])
    {
      pass = false;
      break;
    }
  }

  // Full format if signature is not present
  if(!pass)
    dataFormat(autoWait);
}

void NVM::dataFormat(bool autoWait)
{
  // Erase Axillary Banks
  for(var8 i = 0; i < auxNvmBlocks; i++)
  {
    nvmEraseBlock(1, i, true);
  }

  // Erase SEEPROM
  volatile var8* seepromPtr = ptr8(seepromOffset);
  for(var32 i = 0; i < seepromSize; i++) {

    // Flush after every formatted page
    if((i % seepromPageSize) == 0)
      seepromFlush(true);

    // Format to 0xFF
    // This helps save to some extent the underlying NVM
    // bits. 0xFF means nothing is written to NVM, only 0's are written
    // It still doesn't prevent the NVM from being erased but it
    // may still help with longevity and wear.
    seepromPtr[i] = 0xFF;
  }

  // Do a final flush of SEEPROM to NVM
  seepromFlush(true);

  for(var8 i = 0; i < auxSignatureLen; i++)
  {
    auxSig[i] = auxSigCompare[i];
  }

  // Save signature update to disk
  seepromFlush(autoWait);
}

void NVM::dataSaveState(bool autoWait)
{
  // Some prep work converting linker markers to usable addresses
  // and additional prep work.
  var8* stateSeeprom = (var8*)(&_sbkupramsave);
  var8* stateBkup = (var8*)(&_sbkupram);
  var8* stateSeepromEnd = (var8*)(&_ebkupramsave);
  var32 stateSize = (var32)(stateSeepromEnd) - (var32)(stateSeeprom);

  for(var32 i = 0; i < stateSize; i++)
  {
    // Copy byte-by-byte from bkupram to seeprom
    stateSeeprom[i] = stateBkup[i];

    // Flush every page size
    if((i % seepromPageSize) == 0)
      seepromFlush(true);
  }

  // Do a final flush
  seepromFlush(autoWait);
}

void NVM::dataRestoreState()
{
  // Some prep work converting linker markers to usable addresses
  // and additional prep work.
  var8* stateSeeprom = (var8*)(&_sbkupramsave);
  var8* stateBkup = (var8*)(&_sbkupram);
  var8* stateSeepromEnd = (var8*)(&_ebkupramsave);
  var32 stateSize = (var32)(stateSeepromEnd) - (var32)(stateSeeprom);

  for(var32 i = 0; i < stateSize; i++)
  {
    // Copy byte-by-byte from seeprom to bkupram
    stateBkup[i] = stateSeeprom[i];
  }
}

var8 __attribute__((section(".seeprom"))) NVM::auxSig[11];
