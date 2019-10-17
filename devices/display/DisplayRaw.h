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
  void update(bool autoSend = false);

  // Sends presentation area to device
  void send();

  // Reset this class instance and send changes to device
  void reset(bool autoUpdate = false, bool autoSend = false);

  // Digits 0-3
  // ----4321
  void setDigit(var8 ind, bool power);
  bool getDigit(var8 ind);

  // Digit Segments 0-7 (A-G + DP)
  // PGFEDCBA
  void setSeg(var8 ind, bool power);
  void setChar(var8 ch);
  bool getSeg(var8 ind);

  // Colon
  void setCol(bool power);
  bool getCol();

  // Replaces a charmap entry with another charmap entry
  static void modifyCharmap(var8 ch, var8 data);

   var8 digits;
   var8 segs;
   bool colon;

   var8 digitsRender;
   var8 segsRender;
   bool colonRender;

   static DisplayRaw inst;

   static var8 charmap[0xFF];
   static var8 formatCode;
   static constexpr var8 formatCodeCompare = 0xA0;

   static void initCharMap();

private:
  bool _cmp(var8 val, var8 bit);
  void  _out(var8 port, var32 pin, bool val);
  void _outPos(var8 port, var32 pin);
  void _outNeg(var8 port, var32 pin);
};

#endif //__DISPLAYRAW_H__
