/*
* RTClock.h
*
* Created: 10/7/2019 12:38:39 AM
* Author: juneh
*/

#ifndef __RTCLOCK_H__
#define __RTCLOCK_H__

#include "../../common/types.h"

class RTClock
{
public:
  static void __attribute__((cold)) boot();

  static var8 getHour();
  static var8 getMinute();
};

#endif
