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
* Boot.h
*
* Created: 9/26/2019 6:07:23 PM
* Author: juneh
*/


#ifndef __CLOCKS_H__
#define __CLOCKS_H__

#include "sam.h"
#include "../../common/types.h"

/*
 * Clocks
 * OSC32K     32.768KHz     Standby
 * OSCULP32K  ~32.768KHz    Standby
 * DFLL       48MHz         Standby
 * DPLL0      120MHz        Standby
 * DPLL1      100MHz        Standby
*/

/*
* GCLK Generators
* 0    120 MHz     - DPLL 0             Standby       Confirmed Speed
* 1    100 MHz     - DPLL 1             Standby       ~Confirmed Speed (99 MHz)  [against 3]
* 2    48 MHz      - DFLL               Standby       Confirmed Speed
* 3    1 MHz       - DFLL / 48          Standby       ~Confirmed Speed (0-1 MHz)  [against 3]
* 4    32.768 KHz  - 32K                Standby       ~Confirmed Speed (31.??? KHz)  [against 5]
* 5    1.024 KHz   - 32K / 32           Standby       ~Confirmed Speed
* 6    12 MHz      - DFLL / 4                         Confirmed Speed
* 7    32.768 KHz  - Internal 32K       Standby       ~Confirmed Speed (31.??? KHz) [against 5]
* 8
* 9
* 10
* 11
*/

/*
 * Components that have a powered main clock
 *
 * Bridges A-D
 * Device Service Unit / Debug
 * NVM and SEEPROM
 * PAC and Component Errors
 * Cache
 * Direct Memory
 * Clocks (GCLK, MCLK, OSCCTRL [DFLL, DPLL0..1], OSC32K [OSCULP32K])
 * Power Manager
 * Reset
 * Power Supply
 * External Interrupts
 * Real-time Clock
 * Event System
 * Port
 * RAM Error Correction
 * Frequency Generator
 * QSPI
*/

class Clocks
{
public:
  static void boot();
  static void _enable_gen(var8 ind, var8 src, var16 div = 1);
  static void _enable_pch(var8 ind, var8 src);
  static void _enable_dpll(var8 ind, var32 ldr);

private:
  static void boot_osc32k();
  static void boot_dfll();
  static void boot_gens_initial();
  static void boot_dpll();
  static void boot_gens_post();
  static void boot_mclk();
  static void boot_post();
};

#endif //__CLOCKS_H__
