/*
 * qspi.cpp
 *
 * Created: 9/28/2019 5:38:55 PM
 *  Author: juneh
 */

#include "qspi_memory.h"

#include "sam.h"
#include "../../common/data.h"

static var16 txBuffer[300];
static var16 rxBuffer[50];

static var16* txBufferPtr;
static var16* rxBufferPtr;

var32 QspiMemory::dummy = 0;

void QspiMemory::boot()
{
  // Skip clearing out buffers, just mark the first
  // element the end. Setup pointers to start
  txBuffer[0] = QSPI_BUFFER_END;
  rxBuffer[0] = QSPI_BUFFER_END;
  txBufferPtr = txBuffer;
  rxBufferPtr = rxBuffer;

  // Setup QSPI Pins
  /*
   MOSI   PA08	Out
   MISO   PA09	In
   WP     PA10	Out
   HOLD   PA11	Out
   SCK    PB10	Out
   CS     PB11	Out
  */
  _setupPinIn(0, 8); // Along with PMUX this allows bi-directional io
  _setupPinIn(0, 9); // Along with PMUX this allows bi-directional io
  _setupPinIn(0, 10); // Along with PMUX this allows bi-directional io
  _setupPinIn(0, 11); // Along with PMUX this allows bi-directional io
  _setupPinOut(1, 10);
  _setupPinOut(1, 11);

  // De-assert chip mode after each transfer
  QSPI->CTRLB.reg = QSPI_CTRLB_CSMODE_LASTXFER;

  // There's only 1 QSPI Interrupt Line for all interrupts
  // Enable it
  NVIC_EnableIRQ(QSPI_IRQn);

  // Enable QSPI
  QSPI->CTRLA.bit.ENABLE = true;

  // Ensure QSPI is configured properly on it's end
  //ensureProperConfig();

  // Load up read instruction
  instrRead();
}

void QspiMemory::modeSPI()
{
  QSPI->CTRLB.bit.MODE = false;
}

void QspiMemory::modeMemory()
{
  QSPI->CTRLB.bit.MODE = true;
}

void QspiMemory::speedLow()
{
  speedAt(4000000);
}

void QspiMemory::speedHigh()
{
  speedAt(120000000);
}

void QspiMemory::speedAt(var32 speed)
{
  QSPI->BAUD.bit.BAUD = (120000000 / speed) - 1;
}

void QspiMemory::cmdHpm()
{
  txStart();
  tx(0xA3, 3);
  execute();
  pollReady();
}

void QspiMemory::cmdWriteEnable()
{
  txStart();
  tx(0x06);
  execute();
  pollReady();
}

void QspiMemory::cmdWriteDisable()
{
  txStart();
  tx(0x04);
  execute();
  pollReady();
}

// Erases the entire chip, takes a very long time
void QspiMemory::cmdCE()
{
  txStart();
  tx(0xC7);
  execute();
  pollReady();
}

void QspiMemory::cmdResetEnable()
{
  txStart();
  tx(0x66);
  execute();
  pollReady();
}

void QspiMemory::cmdReset()
{
  txStart();
  tx(0x99);
  execute();
  pollReady();
}

void QspiMemory::instrRead()
{
  // End Instruction and switch to auto memory mode
  instrChange();
  speedHigh();
  modeMemory();

  QSPI->INSTRCTRL.bit.INSTR = 0x6B;
  QSPI->INSTRFRAME.reg =
    QSPI_INSTRFRAME_DUMMYLEN(8) | QSPI_INSTRFRAME_TFRTYPE_READMEMORY | QSPI_INSTRFRAME_DATAEN |
    QSPI_INSTRFRAME_ADDREN | QSPI_INSTRFRAME_INSTREN | QSPI_INSTRFRAME_WIDTH_QUAD_OUTPUT;

  _instrReadSync();
}

var8 QspiMemory::readStatusLow()
{
  txStart();
  tx(0x05);
  execute();
  return *rxBufferPtr;
}

var8 QspiMemory::readStatusHigh()
{
  txStart();
  tx(0x35);
  execute();
  return *rxBufferPtr;
}

void QspiMemory::instrWriteStatus(var16 status)
{
  // Split transmission over several bytes
  txStart();
  tx(0x01);
  tx(status & 0xFF, 0);
  tx((status & 0xFF00) >> 8);
  execute();
  pollReady();
}

void QspiMemory::instrWritePage(var32 addr, var8* data, var32 len, bool autoTrans /* = true*/)
{
  txStart();

  // Get address to page but not from our memory map
  // From the Memory Devices memory map
  if(autoTrans)
    addr -= offset;

  // Send page program + address and leave open
  tx(0x02);
  tx((addr & 0xFF0000) >> 16);
  tx((addr & 0x00FF00) >> 8);
  tx(addr & 0x0000FF);

  for(var32 i = 0; i < len; i++)
  {
    tx(data[i]);
  }

  execute();
  pollReady();
}

void QspiMemory::instrEraseSector(var32 addr, var8 block, var8 sector, bool autoTrans /*= true*/)
{
  txStart();

  // Get address to sector but not from our memory map
  // From the Memory Devices memory map
  if(autoTrans)
    addr -= offset;

  // Send sector erase + address
  tx(0x20);
  tx((addr & 0xFF0000) >> 16);
  tx((addr & 0x00FF00) >> 8);
  tx(addr & 0x0000FF);

  execute();
  pollReady();
}

void QspiMemory::instrEraseBlock(var32 addr, var8 block, bool autoTrans /*= true*/)
{
  txStart();

  // Get address to sector but not from our memory map
  // From the Memory Devices memory map
  if(autoTrans)
    addr -= offset;

  // Send sector erase + address
  tx(0xD8);
  tx((addr & 0xFF0000) >> 16);
  tx((addr & 0x00FF00) >> 8);
  tx(addr & 0x0000FF);

  execute();
  pollReady();
}

// Changes mode over to a new instruction if there's a current open one
void QspiMemory::instrChange()
{
  // If the chip select line is not up
  // do nothing
  if(QSPI->STATUS.bit.CSSTATUS == true)
    return;

  // Mark end of instruction
  QSPI->CTRLA.bit.LASTXFER = true;

  // Wait for it to end
  while(QSPI->STATUS.bit.CSSTATUS != true);
}

void QspiMemory::pollReady()
{
  var8 data = readStatusLow();

  while(true)
  {
    if((data & 1) == 0)
      break;

    // Read Again
    data = readStatusLow();
  }
}

void QspiMemory::ensureProperConfig()
{
  // Look at status of QE bit and HPF bit, they both need to be one
  // QF = Quad IO Enable
  // HPF = High-Performance Enable
  var8 statusHigh = readStatusHigh();
  statusHigh &= 0b00100010;

  // If both flags are set then stop here
  if(statusHigh == 0b00100010)
    return;

  // Grab the lower status registers
  var8 statusLow = readStatusLow();

  // Merge in flags that need to be set
  statusHigh |= 0b00100010;

  // Combine the two into one status register
  var16 st = (statusHigh << 8) | statusLow;

  // Open up QSPI for writing and write into the status registers
  cmdWriteEnable();
  instrWriteStatus(st);
  cmdWriteDisable();

  cmdHpm(); // Enable High Performance Mode
}

void QspiMemory::tx(var8 data, var8 dummy /*= 0*/)
{
  *txBufferPtr = data;
  txBufferPtr++;

  txDummy(dummy);
}

// Execute the transmission buffer and receive contents into the receive buffer
// as they arrive
void QspiMemory::execute()
{
  // Mark end of buffer automatically
  *txBufferPtr = QSPI_BUFFER_END;

  // Setup
  // Adjust speed very low, ensure current instruction has ended, switch to manual mode
  speedLow();
  instrChange();
  modeSPI();

  // Place pointers at top
  txBufferPtr = txBuffer;
  rxBufferPtr = rxBuffer;

  // Execute List and block until done
  completion = false;
  QSPI->INTENSET.bit.DRE = true;
  QSPI->INTENSET.bit.RXC = true;
  while(completion == false);

  // Re-place pointers at top and end instruction
  txStart();
  instrChange();
}

void QspiMemory::txStart()
{
  txBufferPtr = txBuffer;
  rxBufferPtr = rxBuffer;
}

void QspiMemory::txDummy(var8 count)
{
  for(var8 i = 0; i < count; i++)
  {
    *txBufferPtr = 0xFF;
    txBufferPtr++;
  }
}

bool QspiMemory::completion = true;

void QspiMemory::_setupPinOut(var8 port, var8 pin)
{
  PORT->Group[port].DIRSET.bit.DIRSET = (1 << pin);
  PORT->Group[port].OUTCLR.bit.OUTCLR = (1 << pin);
  PORT->Group[port].PINCFG[pin].reg = PORT_PINCFG_PMUXEN;

  if(pin % 2 == 0)
    PORT->Group[port].PMUX[pin / 2].bit.PMUXE = 7;
  else
    PORT->Group[port].PMUX[pin / 2].bit.PMUXO = 7;
}

void QspiMemory::_setupPinIn(var8 port, var8 pin)
{
  PORT->Group[port].CTRL.bit.SAMPLING = (1 << pin);
  PORT->Group[port].DIRCLR.bit.DIRCLR = (1 << pin);
  PORT->Group[port].OUTCLR.bit.OUTCLR = (1 << pin);
  PORT->Group[port].PINCFG[pin].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN;

  if(pin % 2 == 0)
    PORT->Group[port].PMUX[pin / 2].bit.PMUXE = 7;
  else
    PORT->Group[port].PMUX[pin / 2].bit.PMUXO = 7;
}

void QspiMemory::_instrReadSync()
{
  dummy = QSPI->INSTRFRAME.reg;
}

void doCompletion()
{
  QspiMemory::completion = true;
  QSPI->INTFLAG.reg = QSPI_INTFLAG_DRE | QSPI_INTFLAG_RXC;
  QSPI->INTENCLR.reg = QSPI_INTENCLR_DRE | QSPI_INTENCLR_RXC;

  // Mark the end of received data
  *rxBufferPtr = QSPI_BUFFER_END;
}

void readyTx()
{
  // If completion then
  // This is a false write, don't actually send it
  if(QspiMemory::completion) {
    QSPI->INTFLAG.reg = QSPI_INTFLAG_DRE | QSPI_INTFLAG_RXC;
    QSPI->INTENCLR.reg = QSPI_INTENCLR_DRE | QSPI_INTENCLR_RXC;
    return;
  }

  // If we're at end then stop here
  if(*txBufferPtr == QSPI_BUFFER_END)
  {
    doCompletion();
    return;
  }

  // Otherwise send byte, move pointer down, and clear flag
  QSPI->TXDATA.bit.DATA = (var8)(*txBufferPtr);
  txBufferPtr++;
  QSPI->INTFLAG.bit.DRE = true;
}


void readyRx()
{
  // If completion then
  // This is a false read, don't actually save it
  if(QspiMemory::completion) {
    QSPI->INTFLAG.reg = QSPI_INTFLAG_DRE | QSPI_INTFLAG_RXC;
    QSPI->INTENCLR.reg = QSPI_INTENCLR_DRE | QSPI_INTENCLR_RXC;
    return;
  }

  // Retrieve data, move pointer down, and clear flag
  *rxBufferPtr = QSPI->RXDATA.bit.DATA;
  rxBufferPtr++;
  QSPI->INTFLAG.bit.RXC = true;
}

extern "C"
{
  void QSPI_Handler()
  {
    if(QSPI->INTFLAG.bit.DRE)
      readyTx();

    if(QSPI->INTFLAG.bit.RXC)
      readyRx();
  }
};
