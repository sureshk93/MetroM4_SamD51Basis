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
* ClockFreqm.cpp
*
* Created: 9/27/2019 1:16:38 PM
* Author: juneh
*/

#include "clock_freqm.h"
#include "../clocks/clocks.h"
#include "../../common/named_gens.h"
#include "../../common/named_phctrl.h"

svar32 ClockFreqm::meausre(var8 ref, var8 to)
{
  // Enable Main Clock
  MCLK->APBAMASK.bit.FREQM_ = true;

  // Enable frequency clocks
  Clocks::_enable_pch(PHCTRL_FREQM_REF, ref);
  Clocks::_enable_pch(PHCTRL_FREQM_MSR, to);

  FREQM->CFGA.bit.REFNUM = 1; // 1 rotation

  // Enable
  FREQM->CTRLA.bit.ENABLE = true;
  while(FREQM->SYNCBUSY.bit.ENABLE);

  // Start
  FREQM->CTRLB.bit.START = true;
  while(FREQM->STATUS.bit.BUSY);

  // Determine result
  bool overflow = FREQM->STATUS.bit.OVF;
  var32 res = FREQM->VALUE.reg;

  // Disable Frequency Clocks
  GCLK->PCHCTRL[PHCTRL_FREQM_REF].reg  =  0;
  while(GCLK->PCHCTRL[PHCTRL_FREQM_REF].bit.CHEN  ==  true);

  GCLK->PCHCTRL[PHCTRL_FREQM_MSR].reg  =  0;
  while(GCLK->PCHCTRL[PHCTRL_FREQM_MSR].bit.CHEN  ==  true);

  // Disable Main Clock
  MCLK->APBAMASK.bit.FREQM_ = false;

  if(overflow)
    return -1;
  else
    return res;
}
