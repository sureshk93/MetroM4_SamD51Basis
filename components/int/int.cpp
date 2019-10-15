/*
 * eic.cpp
 *
 * Created: 9/27/2019 3:43:25 PM
 *  Author: juneh
 */

#include "sam.h"
#include "int.h"
#include "../../common/types.h"

void Int::boot()
{
  // Enable and use GCLK Clock
  EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
  while(EIC->SYNCBUSY.bit.ENABLE);

  // Enable Master Interrupts
  __enable_irq();
}
