/*
* Supply.h
*
* Created: 10/7/2019 8:16:40 AM
* Author: juneh
*/

#ifndef __SUPPLY_H__
#define __SUPPLY_H__

class Supply
{
public:
  static void __attribute__((cold)) boot();

  static void powerCPUTemp();
  static void powerCompTemp();
};

#endif
