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
