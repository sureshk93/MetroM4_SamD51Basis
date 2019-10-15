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
* ClockFreqm.h
*
* Created: 9/27/2019 1:16:38 PM
* Author: juneh
*/

#include "../../common/types.h"

#ifndef __CLOCKFREQM_H__
#define __CLOCKFREQM_H__

class ClockFreqm
{
public:
  // Max Ref 100 MHz
  // Max To 200 MHZ
  static svar32 __attribute__((cold)) meausre(var8 ref, var8 to);
};

#endif //__CLOCKFREQM_H__
