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
* Timers.h
*
* Created: 10/5/2019 2:08:15 AM
* Author: juneh
*/

/*
 * Timers
 * TC0    16-bit    1MHz    Triggers every 5,000x (5ms)
*/

#ifndef __TIMERS_H__
#define __TIMERS_H__

#include "../../common/types.h"

class Timers
{
public:
  static void boot();
};

#endif //__TIMERS_H__
