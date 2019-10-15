/*
 * scratch.cpp
 *
 * Created: 10/4/2019 8:11:01 PM
 *  Author: juneh
 */

//#include "./boot/clocks.h"
//#include "./boot/QspiMemory.h"
//#include "./common/types.h"
//#include "./common/data.h"
//#include "./common/named_gens.h"
//#include "./common/named_phctrl.h"
//#include "./util/ClockFreqm.h"
//#include "./devices/display/DisplaySimple.h"
//
//void i2c_setup_pin_out(var8 port, var8 pin)
//{
  //PORT->Group[port].DIRSET.bit.DIRSET = (1 << pin);
  //PORT->Group[port].OUTCLR.bit.OUTCLR = (1 << pin);
  //PORT->Group[port].PINCFG[pin].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_DRVSTR;
//
  //if(pin % 2 == 0)
    //PORT->Group[port].PMUX[pin / 2].bit.PMUXE = 3;
  //else
    //PORT->Group[port].PMUX[pin / 2].bit.PMUXO = 3;
//}
//
//void i2c_setup_pin_in(var8 port, var8 pin)
//{
  //PORT->Group[port].CTRL.bit.SAMPLING = (1 << pin);
  //PORT->Group[port].DIRCLR.bit.DIRCLR = (1 << pin);
  //PORT->Group[port].OUTCLR.bit.OUTCLR = (1 << pin);
  //PORT->Group[port].PINCFG[pin].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN | PORT_PINCFG_DRVSTR;
//
  //if(pin % 2 == 0)
    //PORT->Group[port].PMUX[pin / 2].bit.PMUXE = 3;
  //else
    //PORT->Group[port].PMUX[pin / 2].bit.PMUXO = 3;
//}
//
//var16 txBuffer[] {
  ////0x00, // Register IODIRA
  ////0x00, // All Out
  ////0x101, // Restart
//
  ////0x01, // Register IODIRB
  ////0x00, // All Out
  ////0x101, // Restart
//
  ////0x12, // GPIO A
  ////0x00, // All Negative
  ////0x101, // Restart
//
  //0x13, // GPIO B
  //0xFF, // All Positive
  //0x100,// EOL
//};
//
//var16* txBufferPtr = txBuffer;
//
//// Enabled if we are to issue a restart command
//var8 rst = 0;
//
//var8 dbgCtr = 0;
//
//void i2C_tx_next()
//{
  //// First check to see if there is an existing restart, if so increment it out of bounds
  //if(rst >= 1)
  //rst++;
//
  //// Next check what the underlying byte is in the pointer
//
  //// Restart if we're at end of transaction
  //if(*txBufferPtr == 0x101)
  //{
    //// Move pointer forward
    //txBufferPtr++;
//
    //// Mark to issue a restart
    //rst++;
  //}
//
  //// Stop if we're at the end of list
  //else if(*txBufferPtr == 0x100)
  //{
    //rst = 0;
    //SERCOM5->I2CM.CTRLB.bit.CMD = 3;
    //while(SERCOM5->I2CM.SYNCBUSY.bit.SYSOP);
    //return;
  //}
//
  //// Grab underlying byte
  //var8 txBufferPtrVal = (var8)*txBufferPtr;
  //dbgCtr++;
//
  //// Set as new data
  //SERCOM5->I2CM.DATA.reg = txBufferPtrVal;
  //while(SERCOM5->I2CM.SYNCBUSY.bit.SYSOP);
//
  //// Increment to next byte
  //txBufferPtr++;
//
  //// Issue restart command if restart is 1 (Valid)
  //if(rst == 1) {
    //SERCOM5->I2CM.CTRLB.bit.CMD = 1;
    //while(SERCOM5->I2CM.SYNCBUSY.bit.SYSOP);
  //}
//
  //// Remove restart command if restart is greater than 1 (Invalid)
  //else if(rst >= 1) {
    //rst = 0;
    //SERCOM5->I2CM.CTRLB.bit.CMD = 2;
    //while(SERCOM5->I2CM.SYNCBUSY.bit.SYSOP);
  //}
//}
//
//extern "C"
//{
  //// A byte has been transmitted, awaiting further instructions for the on-going transaction
  //void SERCOM5_0_Handler()
  //{
    //// Ensure there's no error, filter out busstate and clkhold
    //// Transmit next byt in list
    //var32 status = SERCOM5->I2CM.STATUS.reg & ~SERCOM_I2CM_STATUS_BUSSTATE_Msk & ~(1 << SERCOM_I2CM_STATUS_CLKHOLD_Pos);
    //if(status == 0)
    //i2C_tx_next();
//
    //// Clear flag
    //SERCOM5->I2CM.INTFLAG.bit.MB = true;
  //}
//
  //void SERCOM5_3_Handler()
  //{
    //// What error happened
    //bool lenErr = SERCOM5->I2CM.STATUS.bit.LENERR;
    //bool sExTTOut = SERCOM5->I2CM.STATUS.bit.SEXTTOUT;
    //bool mExTTOut = SERCOM5->I2CM.STATUS.bit.MEXTTOUT;
    //bool clkHold = SERCOM5->I2CM.STATUS.bit.MEXTTOUT;
    //bool lowTOut = SERCOM5->I2CM.STATUS.bit.LOWTOUT;
    //var8 busState = SERCOM5->I2CM.STATUS.bit.BUSSTATE;
    //bool rxNAck = SERCOM5->I2CM.STATUS.bit.RXNACK;
    //bool arbLost = SERCOM5->I2CM.STATUS.bit.ARBLOST;
    //bool busErr = SERCOM5->I2CM.STATUS.bit.BUSERR;
//
    //SERCOM5->I2CM.INTFLAG.bit.ERROR = true;
  //}
//};
//
//// Enable MCLK Clock
  ////MCLK->APBDMASK.bit.SERCOM5_ = true;
//
  //// Sercom 5 Pad 0:1 (SCL [PB03] & SDA [PB02] ) Lines
  ////i2c_setup_pin_in(1, 3); // Clock Out
  ////i2c_setup_pin_in(1, 2); // IO Line
//
  //// Enable SERCOM 5 to be at 1MHz, the maximum I2C Speed supported by both devices
  ////Clocks::_enable_pch(PHCTRL_SERCOM5_CORE, GEN_1MHz);
//
  //// Speed is FM+ (1MHz)
  //// SDA Hold down ~75ns [50ns-100ns]
  //// Master Mode
  ////SERCOM5->I2CM.CTRLA.reg = SERCOM_I2CM_CTRLA_SPEED(1) | SERCOM_I2CM_CTRLA_SDAHOLD(0) | SERCOM_I2CM_CTRLA_MODE(5) |
                            ///*SERCOM_I2CM_CTRLA_LOWTOUTEN |*/ SERCOM_I2CM_CTRLA_INACTOUT(3);
//
  //// Restart cmd
  //// Enable Smart Mode (To handle some stuff for us)
  ////SERCOM5->I2CM.CTRLB.reg = /*SERCOM_I2CM_CTRLB_CMD(1) |*/ SERCOM_I2CM_CTRLB_SMEN;
//
  //// No Baud, already working at 1MHz, any lower is fine
  //// Setting to 1 anyways for FM+ Mode
  ////SERCOM5->I2CM.BAUD.reg = SERCOM_I2CM_BAUD_BAUD(0) | SERCOM_I2CM_BAUD_BAUDLOW(1);
//
  //// Enable Error and MB
  //// MB = A byte is transmitted with or without error
  //// SB = A byte is successfully received
  ///*
   //* Need to clear I2C Line with one of these (Status.CLKHOLD)
   //* Writing to ADDR.ADDR
   //* Writing to DATA.DATA
   //* Reading DATA.DATA when smart mode is enabled (CTRLB.SMEN)
   //* Writing a valid command to CTRLB.CMD
  //*/
  ////SERCOM5->I2CM.INTENSET.reg = SERCOM_I2CM_INTENSET_ERROR | SERCOM_I2CM_INTENSET_MB;
  ////NVIC_EnableIRQ(SERCOM5_0_IRQn);
  ////NVIC_EnableIRQ(SERCOM5_3_IRQn);
//
  //// Load up first transaction data byte
  ////i2C_tx_next();
//
  //// Enable I2C
  ////SERCOM5->I2CM.CTRLA.bit.ENABLE = true;
  ////while(SERCOM5->I2CM.SYNCBUSY.bit.ENABLE);
//
  //// Wait for bus ready
  ////while(SERCOM5->I2CM.STATUS.bit.BUSSTATE != 1);
//
  //// 7-bit address in write mode
  ////SERCOM5->I2CM.ADDR.bit.ADDR = (0x20 << 1) | 0;
  ////while(SERCOM5->I2CM.SYNCBUSY.bit.SYSOP);
