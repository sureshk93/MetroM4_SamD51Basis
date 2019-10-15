/*
* Alarm.cpp
*
* Created: 10/7/2019 4:42:41 AM
* Author: juneh
*/

#include "alarm.h"
#include "sam.h"

// Setup pins
void Alarm::init()
{
  // Off Button PA20 (D9)
  // Alarm Light PA18 (D10)
  PORT->Group[0].CTRL.reg |= (1 << 20);
  PORT->Group[0].DIRCLR.reg = (1 << 20);
  PORT->Group[0].OUTCLR.reg = (1 << 20);
  PORT->Group[0].PINCFG[20].bit.INEN = true;

  PORT->Group[0].DIRSET.reg = (1 << 18);
  PORT->Group[0].OUTCLR.reg = (1 << 18);
}

// Enable Alarm Light
void Alarm::alarmFlagSet()
{
  PORT->Group[0].OUTSET.reg = (1 << 18);
}

// Disable Alarm Light
void Alarm::alarmFlagClr()
{
  PORT->Group[0].OUTCLR.reg = (1 << 18);
}

// Disable alarm light if alarm button pressed
void Alarm::alarmBtnSample()
{
  if(PORT->Group[0].IN.bit.IN & (1 << 20))
    alarmFlagClr();
}
