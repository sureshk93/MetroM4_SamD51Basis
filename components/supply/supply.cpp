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
