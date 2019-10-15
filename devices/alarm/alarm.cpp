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
