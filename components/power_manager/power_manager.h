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
* PowerManager.h
*
* Created: 10/6/2019 5:42:39 PM
* Author: juneh
*/

/*
 * PDCORESW - CPU and all Components except backup components
 * PDBACKUP - Backup Components (OSC32KCTRL, SUPC, RSTC, RTC and the PM itself)
 * PDSYSRAM - System RAM (Configurable Power)
 * PDBKUPRAM - Backup RAM (Configurable Power)
*/

#ifndef __POWERMANAGER_H__
#define __POWERMANAGER_H__

#include "../../common/types.h"

class PowerManager
{
public:
  static void __attribute__((cold)) boot();

  static void enterLastMode();

  /*
   * Idle:
   * CPU Stopped
   * Synchronous Cloks Stopped unless requested
   * Logic Retained
  */
  static void enterIdle();

  /*
   * Standby (Sleep Walking)
   * CPU and Components Stopped
   * Logic Retained
   * SRAM power can be configured
  */
  static void __attribute__((hot)) enterStandby();

  /*
   * Hibernate (IO Rentention)
   * PDCORESW powered off
   * Backup power is kept
   * Backup RAM Configurable
  */
  static void enterHibernate();

  /*
   * Backup (IO Rentention)
   * All but backup is shutdown
   * Backup RAM can be configured
  */
  static void enterBackup();

  /*
   * Device is shutdown
  */
  static void enterOff();

private:
  static void __attribute__((hot)) _switchMode(var32 mode);
};

#endif
