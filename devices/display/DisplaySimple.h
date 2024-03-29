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
* DisplaySimple.h
*
* Created: 10/4/2019 6:17:03 PM
* Author: juneh
*/

#ifndef __DISPLAYSIMPLE_H__
#define __DISPLAYSIMPLE_H__

#include "./DisplayRaw.h"
#include "../../common/types.h"

class DisplaySimple
{
public:
  DisplaySimple(bool autoUpdate = true, bool autoSend = false);

  // Copy working area to presentation area and send
  void update(bool autoSend = false);
  void updateOne(var8 ind, bool autoSend = false);

  // Sends presentation area to device
  void send(var8 ind);

  // Sends next digit, cycles around 0-4
  void sendNext();

  // Reset this class instance and send changes to device
  void reset(bool autoUpdate = false, bool autoSend = false);

  // Digits 0-3
  // ----4321
  void setDigit(var8 ind, bool power);
  bool getDigit(var8 ind);

  // Digit Segments 0-7 (A-G + DP)
  // PGFEDCBA
  void setSeg(var8 digit, var8 ind, bool power);
  void setChar(var8 digit, var8 ch);
  bool getSeg(var8 digit, var8 ind);

  void setStr(const var8* str);
  void setStr(const char* str);

  // Colon
  void setCol(bool power);
  bool getCol();
  void doTick();

  void doClockUpdate();

  // Get first digit raw
  // Each digit can actually be used as a layer
  // This returns the bottom-most layer in raw form
  DisplayRaw getRaw();

  DisplayRaw digits[4];
  var8 digitNext;
  static constexpr var8 digitCount = 4;
  static DisplaySimple inst;

  bool autoClockEn = false;
  bool autoTickEn = false;
  bool autoSendEn = true;
};

#endif //__DISPLAYSIMPLE_H__
