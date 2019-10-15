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
* Timers.cpp
*
* Created: 10/5/2019 2:08:15 AM
* Author: juneh
*/

#include "timers.h"
#include "sam.h"

void Timers::boot()
{
  // Timer 0 16-bit (1MHz Clock)
  // Interrupt every 5000x (5ms)
  // Always run even in standby and debug
  TC0->COUNT16.CTRLA.reg = TC_CTRLA_RUNSTDBY;
  TC0->COUNT16.INTENSET.reg = TC_INTENSET_MC0;
  TC0->COUNT16.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;
  TC0->COUNT16.DBGCTRL.reg = TC_DBGCTRL_DBGRUN;

  TC0->COUNT16.CC[0].bit.CC = 5000;
  while(TC0->COUNT16.SYNCBUSY.bit.CC0);

  NVIC_EnableIRQ(TC0_IRQn);
  TC0->COUNT16.CTRLA.bit.ENABLE = true;
  while(TC0->COUNT16.SYNCBUSY.bit.ENABLE);

  TC0->COUNT16.CTRLBSET.bit.CMD = TC_CTRLBSET_CMD_RETRIGGER_Val;
  while(TC0->COUNT16.SYNCBUSY.bit.CTRLB);
}
