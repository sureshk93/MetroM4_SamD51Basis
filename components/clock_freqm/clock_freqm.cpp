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
