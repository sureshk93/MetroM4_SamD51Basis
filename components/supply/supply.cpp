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
* Supply.cpp
*
* Created: 10/7/2019 8:16:40 AM
* Author: juneh
*/

#include "supply.h"
#include "sam.h"

void Supply::boot()
{
  // Enable CPU Temp and route it to ADC for measuring
  SUPC->VREF.reg = SUPC_VREF_TSSEL | SUPC_VREF_TSEN;
}

void Supply::powerCPUTemp()
{
  SUPC->VREF.bit.TSSEL = 1;
}

void Supply::powerCompTemp()
{
  SUPC->VREF.bit.TSSEL = 0;
}
