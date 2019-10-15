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
