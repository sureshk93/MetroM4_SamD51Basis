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
* PowerManager.cpp
*
* Created: 10/6/2019 5:42:39 PM
* Author: juneh
*/

#include "power_manager.h"
#include "sam.h"
#include "../clocks/clocks.h"

void PowerManager::boot()
{
  // Ready default sleep mode standby
  PM->SLEEPCFG.reg = PM_SLEEPCFG_SLEEPMODE_STANDBY;

  // Configure Standby
  PM->STDBYCFG.bit.FASTWKUP = 3; // Fully use fast wake-up
}

void PowerManager::enterLastMode()
{
  // Sleep
  __WFI();
}

void PowerManager::enterIdle()
{
  _switchMode(2);
  enterLastMode();
}

void PowerManager::enterStandby()
{
  _switchMode(4);
  enterLastMode();
}

void PowerManager::enterHibernate()
{
  _switchMode(5);
  enterLastMode();
}

void PowerManager::enterBackup()
{
  _switchMode(6);
  enterLastMode();
}

void PowerManager::enterOff()
{
  _switchMode(7);
  enterLastMode();
}

void PowerManager::_switchMode(var32 mode)
{
  // Check for current mode, if not in current mode then switch and wait
  if(PM->SLEEPCFG.reg != mode) {
    PM->SLEEPCFG.reg = mode;
    while(PM->SLEEPCFG.reg != mode);
  }
}
