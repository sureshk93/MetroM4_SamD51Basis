/*
* DisplayRaw.h
*
* Created: 10/4/2019 5:13:34 PM
* Author: juneh
*/

#ifndef __DISPLAYRAW_H__
#define __DISPLAYRAW_H__

#include "../../common/types.h"

class DisplayRaw
{
public:
  DisplayRaw(bool autoUpdate = true, bool autoSend = false);

  // Copy working area to presentation area and send
  void __attribute__((hot)) update(bool autoSend = false);

  // Sends presentation area to device
  void __attribute__((hot)) send();

  // Reset this class instance and send changes to device
  void reset(bool autoUpdate = false, bool autoSend = false);

  // Digits 0-3
  // ----4321
  void __attribute__((hot)) setDigit(var8 ind, bool power);
  bool getDigit(var8 ind);

  // Digit Segments 0-7 (A-G + DP)
  // PGFEDCBA
  void __attribute__((hot)) setSeg(var8 ind, bool power);
  void __attribute__((hot)) setChar(var8 ch);
  bool getSeg(var8 ind);

  // Colon
  void __attribute__((hot)) setCol(bool power);
  bool __attribute__((hot)) getCol();

  // Replaces a charmap entry with another charmap entry
  static void modifyCharmap(var8 ch, var8 data);

   var8 digits;
   var8 segs;
   bool colon;

   var8 digitsRender;
   var8 segsRender;
   bool colonRender;

   static void initCharMap();

private:
  bool __attribute__((hot)) _cmp(var8 val, var8 bit);
  void __attribute__((hot)) _out(var8 port, var32 pin, bool val);
  void __attribute__((hot)) _outPos(var8 port, var32 pin);
  void __attribute__((hot)) _outNeg(var8 port, var32 pin);
};

extern DisplayRaw displayRaw;

#endif //__DISPLAYRAW_H__
