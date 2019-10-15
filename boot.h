/*
 * boot.h
 *
 * Created: 9/26/2019 11:39:03 PM
 *  Author: juneh
 */


#ifndef BOOT_H_
#define BOOT_H_

#include "sam.h"

class Boot
{
public:
  void static __attribute__((cold)) boot();
};

#endif /* BOOT_H_ */
