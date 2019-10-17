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
 * eic.cpp
 *
 * Created: 9/27/2019 3:43:25 PM
 *  Author: juneh
 */

#include "sam.h"
#include "int.h"
#include "../../common/types.h"

void __attribute__((cold)) Int::boot()
{
  enableInt(true);
}

void Int::enableInt(bool enable)
{
  // Skip if already enabled
  if(EIC->CTRLA.bit.ENABLE == enable)
    return;

  // Enable and use GCLK Clock
  EIC->CTRLA.reg = (enable) ? EIC_CTRLA_ENABLE : 0;
  while(EIC->SYNCBUSY.bit.ENABLE);

  // Enable Master Interrupts
  (enable)
    ? __enable_irq()
    : __disable_irq();
}
