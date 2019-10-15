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
  static void __attribute__((cold)) boot();
};

#endif //__TIMERS_H__
