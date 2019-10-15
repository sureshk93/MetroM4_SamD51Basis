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
 * named_gens.h
 *
 * Created: 9/27/2019 1:27:54 PM
 *  Author: juneh
 */


#ifndef NAMED_GENS_H_
#define NAMED_GENS_H_

#include "types.h"

enum : var8
{
  GEN_120MHz = 0,
  GEN_100MHz,
  GEN_48MHz,
  GEN_1MHz,
  GEN_32_768KHz,
  GEN_1_024KHz,
  GEN_12MHz,
  GEN_32_768KHz_ULP
};

#endif /* NAMED_GENS_H_ */
