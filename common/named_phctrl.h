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
 * named_phctrl.h
 *
 * Created: 9/27/2019 1:27:01 PM
 *  Author: juneh
 */

#ifndef NAMED_PHCTRL_H_
#define NAMED_PHCTRL_H_

#include "types.h"

enum : var8 {
  PHCTRL_OSCCTRL_DFLL48 = 0, //DFLL48 input clock source
  PHCTRL_OSCCTRL_FDPLL0 = 1, //Reference clock for FDPLL0
  PHCTRL_OSCCTRL_FDPLL1 = 2, //Reference clock for FDPLL1
  PHCTRL_OSCCTRL_FDPLL0_32K = 3, // FDPLL0 32KHz clock for internal lock timer
  PHCTRL_OSCCTRL_FDPLL1_32K = 3, // FDPLL1 32KHz clock for internal lock timer
  PHCTRL_SDHC0_SLOW = 3, // SDHC0 Slow
  PHCTRL_SDHC1_SLOW = 3, // SDHC1 Slow
  PHCTRL_SERCOM_0_7__SLOW = 3, // SERCOM[0..7] Slow
  PHCTRL_EIC = 4, //EIC
  PHCTRL_FREQM_MSR = 5, //FREQM Measure
  PHCTRL_FREQM_REF = 6, //FREQM Reference
  PHCTRL_SERCOM0_CORE = 7, //SERCOM0 Core
  PHCTRL_SERCOM1_CORE = 8, //SERCOM1 Core
  PHCTRL_TC0 = 9, //TC0
  PHCTRL_TC1 = 9, //TC1
  PHCTRL_USB = 10, //USB
  PHCTRL_EVSYS_0 = 11, // EVSYS0
  PHCTRL_EVSYS_1 = 12, // EVSYS1
  PHCTRL_EVSYS_2 = 13, // EVSYS2
  PHCTRL_EVSYS_3 = 14, // EVSYS3
  PHCTRL_EVSYS_4 = 15, // EVSYS4
  PHCTRL_EVSYS_5 = 16, // EVSYS5
  PHCTRL_EVSYS_6 = 17, // EVSYS6
  PHCTRL_EVSYS_7 = 18, // EVSYS7
  PHCTRL_EVSYS_8 = 19, // EVSYS8
  PHCTRL_EVSYS_9 = 20, // EVSYS9
  PHCTRL_EVSYS_10 = 21, // EVSYS10
  PHCTRL_EVSYS_11 = 22, // EVSYS11
  PHCTRL_SERCOM2_CORE = 23, //SERCOM2 Core
  PHCTRL_SERCOM3_CORE = 24, //SERCOM3 Core
  PHCTRL_TCC0 = 25, // GCLK_TCC1 TCC0, TCC1
  PHCTRL_TC2 = 26, // GCLK_TC3 TC2, TC3
  PHCTRL_CAN0 = 27, //CAN0
  PHCTRL_CAN1 = 28, //CAN1
  PHCTRL_TCC2 = 29, // GCLK_TCC3 TCC2, TCC3
  PHCTRL_TC4 = 30, // GCLK_TC5 TC4, TC5
  PHCTRL_PDEC = 31, //PDEC
  PHCTRL_AC = 32, //AC
  PHCTRL_CCL = 33, //CCL
  PHCTRL_SERCOM4_CORE = 34, //SERCOM4 Core
  PHCTRL_SERCOM5_CORE = 35, //SERCOM5 Core
  PHCTRL_SERCOM6_CORE = 36, //SERCOM6 Core
  PHCTRL_SERCOM7_CORE = 37, //SERCOM7 Core
  PHCTRL_TCC4 = 38, //TCC4
  PHCTRL_TC6 = 39, // GCLK_TC7 TC6, TC7
  PHCTRL_ADC0 = 40, //ADC0
  PHCTRL_ADC1 = 41, //ADC1
  PHCTRL_DAC = 42, //DAC
  PHCTRL_I2S_0 = 43, // I2S
  PHCTRL_I2S_1 = 44, // I2S
  PHCTRL_SDHC0 = 45, //SDHC0
  PHCTRL_SDHC1 = 46, //SDHC1
  PHCTRL_CM4_TRACE = 47, //CM4 Trace
};

#endif /* NAMED_PHCTRL_H_ */
