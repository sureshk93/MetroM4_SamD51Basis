/*
* NVM.cpp
*
* Created: 10/9/2019 1:28:17 AM
* Author: juneh
*/

#include "nvm.h"
#include "sam.h"

void NVM::boot()
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

void NVM::nvmLowPower()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SPRM;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmHighPower()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_CPRM;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmClearWriteBuffer()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_PBC;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmSetSecurity()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SSB;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmBankSwap()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_BKSWRST;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmDisableCE()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_CELCK;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmEnableCE()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_CEULCK;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmDisableBootProt()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SBPDIS;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmEnableBootProt()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_CBPDIS;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmEraseBlock(var8 bank, var8 block)
{
  NVMCTRL->ADDR.bit.ADDR = getAddrNvmBlock(bank, block);
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_EB;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmLockRegion(var8 bank, var8 region)
{
  NVMCTRL->ADDR.bit.ADDR = getAddrNvmRegion(bank, region);
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_LR;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::nvmUnlockRegion(var8 bank, var8 region)
{
  NVMCTRL->ADDR.bit.ADDR = getAddrNvmRegion(bank, region);
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_UR;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

var32 NVM::nvmStartWritePage(var8 bank, var8 block, var8 page)
{
  nvmClearWriteBuffer();
  var32 addr = getAddrNvmPage(bank, block, page);
  NVMCTRL->ADDR.bit.ADDR = addr;
  return addr;
}

var32 NVM::nvmStartWriteRegionPage(var8 bank, var8 region, var8 block, var8 page)
{
  nvmClearWriteBuffer();
  var32 addr = getAddrNvmRegionPage(bank, region, block, page);
  NVMCTRL->ADDR.bit.ADDR = addr;
  return addr;
}

void NVM::nvmEndWritePage()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_WP;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

var32 NVM::nvmStartWriteQWord(var8 bank, var8 block, var8 page, var8 qword)
{
  nvmClearWriteBuffer();
  var32 addr = getAddrNvmQWord(bank, block, page, qword);
  NVMCTRL->ADDR.bit.ADDR = addr;
  return addr;
}

var32 NVM::nvmStartWriteRegionQWord(var8 bank, var8 region, var8 block, var8 page, var8 qword)
{
  nvmClearWriteBuffer();
  var32 addr = getAddrNvmRegionQWord(bank, region, block, page, qword);
  NVMCTRL->ADDR.bit.ADDR = addr;
  return addr;
}

void NVM::nvmEndWriteQWord()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_WQW;
  while(NVMCTRL->STATUS.bit.READY == 0);
}

void NVM::seepromLock()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_LSEE;
  while(NVMCTRL->STATUS.bit.READY == 0 || NVMCTRL->SEESTAT.bit.BUSY);
}

void NVM::seepromUnlock()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_USEE;
  while(NVMCTRL->STATUS.bit.READY == 0 || NVMCTRL->SEESTAT.bit.BUSY);
}

void NVM::seepromLockReg()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_LSEER;
  while(NVMCTRL->STATUS.bit.READY == 0 || NVMCTRL->SEESTAT.bit.BUSY);
}

void NVM::seepromUnlockReg()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_USEER;
  while(NVMCTRL->STATUS.bit.READY == 0 || NVMCTRL->SEESTAT.bit.BUSY);
}

void NVM::seepromRelocate()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SEERALOC;
  while(NVMCTRL->STATUS.bit.READY == 0 || NVMCTRL->SEESTAT.bit.BUSY);
}

void NVM::seepromFlush()
{
  NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_SEEFLUSH;
  while(NVMCTRL->STATUS.bit.READY == 0 || NVMCTRL->SEESTAT.bit.BUSY);
}

void NVM::seepromSectorChange(var8 sector)
{
  if(sector == 0)
    NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_ASEES0;
  else
    NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_ASEES1;

  while(NVMCTRL->STATUS.bit.READY == 0 || NVMCTRL->SEESTAT.bit.BUSY);
}

void NVM::dataAreaCheck()
{
  // First QWord of Data Bank (Bank B)
  volatile var8* dataPtr = ptr8(getAddrNvmQWord(1, 0, 0, 0));

  bool pass = true; // Optimistic pass strategy
  for(var8 i = 0; i < auxSignatureLen; i++)
  {
    if(dataPtr[i] != auxSignature[i])
    {
      pass = false;
      break;
    }
  }

  // Full format if signature is not present
  if(!pass)
    dataFormat();
}

void NVM::dataFormat()
{
  // Erase Axillary Banks
  for(var8 i = 0; i < auxNvmBlocks; i++)
  {
    nvmEraseBlock(1, i);
  }

  // Erase SEEPROM
  volatile var8* seepromPtr = ptr8(seepromOffset);
  for(var32 i = 0; i < seepromSize; i++) {

    // Flush after every formatted page
    if((i % seepromPageSize) == 0)
      seepromFlush();

    // Format to 0xFF
    // This helps save to some extent the underlying NVM
    // bits. 0xFF means nothing is written to NVM, only 0's are written
    // It still doesn't prevent the NVM from being erased but it
    // may still help with longevity and wear.
    seepromPtr[i] = 0xFF;
  }

  // Do a final flush of SEEPROM to NVM
  seepromFlush();

  // Write signature to indicate a valid format
  // NVM requires a 32-bit write but we use 8-bit reads
  // We must be careful to issue a 32-bit write that reads as intended in 8-bit
  volatile var32* dataPtr = ptr32(nvmStartWriteQWord(1, 0, 0, 0));

  for(var8 i = 0, j = 0; i < auxSignatureLen; i += 4, j++)
  {
    // Reverse order so they can be read correctly in 8-bit mode
    dataPtr[j] = concat8to32(
      auxSignature[i + 0],
      auxSignature[i + 1],
      auxSignature[i + 2],
      auxSignature[i + 3]
    );
  }

  nvmEndWriteQWord();
}