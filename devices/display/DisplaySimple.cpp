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
* DisplaySimple.cpp
*
* Created: 10/4/2019 6:17:03 PM
* Author: juneh
*/

#include "DisplaySimple.h"
#include "../../components/rt_clock/rt_clock.h"

 DisplaySimple::DisplaySimple(bool autoUpdate /*= true*/, bool autoSend /*= false*/)
{
  reset(autoUpdate, autoSend);
}

void  __attribute__((section(".lnl"))) DisplaySimple::update(bool autoSend /*= false*/)
{
  for(var8 i = 0; i < digitCount; i++)
    updateOne(i, autoSend);
}

void  __attribute__((section(".lnl"))) DisplaySimple::updateOne(var8 ind, bool autoSend /*= false*/)
{
  digits[ind].update(autoSend);
}

void  __attribute__((section(".lnl"))) DisplaySimple::send(var8 ind)
{
  digits[ind].send();
}

void  __attribute__((section(".lnl"))) DisplaySimple::sendNext()
{
  if(!autoSendEn)
    return;

  send(digitNext);

  digitNext++;
  if(digitNext >= digitCount)
    digitNext = 0;
}

void DisplaySimple::reset(bool autoUpdate /*= false*/, bool autoSend /*= false*/)
{
  digitNext = 0;
  digits[0] = DisplayRaw();
  digits[1] = DisplayRaw();
  digits[2] = DisplayRaw();
  digits[3] = DisplayRaw();

  for(var8 i = 0; i < digitCount; i++)
  {
    digits[i] = DisplayRaw();
    digits[i].setDigit(i, true);
  }

  if(autoUpdate)
    update(autoSend);
}

void  __attribute__((section(".lnl"))) DisplaySimple::setDigit(var8 ind, bool power)
{
  digits[ind].setDigit(ind, power);
}

bool DisplaySimple::getDigit(var8 ind)
{
  return digits[ind].getDigit(ind);
}

void  __attribute__((section(".lnl"))) DisplaySimple::setSeg(var8 digit, var8 ind, bool power)
{
  digits[digit].setSeg(ind, power);
}

void  __attribute__((section(".lnl"))) DisplaySimple::setChar(var8 digit, var8 ch)
{
  digits[digit].setChar(ch);
}

void  __attribute__((section(".lnl"))) DisplaySimple::setStr(const var8* str)
{
  for(var8 i = 0; i < digitCount; i++)
    digits[i].setChar(str[i]);
}

void  __attribute__((section(".lnl"))) DisplaySimple::setStr(const char* str)
{
  for(var8 i = 0; i < digitCount; i++)
    digits[i].setChar(str[i]);
}

bool DisplaySimple::getSeg(var8 digit, var8 ind)
{
  return digits[digit].getSeg(ind);
}

void DisplaySimple::setCol(bool power)
{
  digits[3].setCol(power);
}

bool DisplaySimple::getCol()
{
  return digits[3].getCol();
}

void DisplaySimple::doTick()
{
  if(!autoTickEn)
    return;

  // Invert colon power and update only the colon
  setCol(!getCol());
  digits[3].colonRender = getCol();
}

void DisplaySimple::doClockUpdate()
{
  if(!autoClockEn)
    return;

  var8 hour = RTClock::getHour();
  var8 min = RTClock::getMinute();

  var8 hour1 = hour / 10;
  if(hour1 == 0)
    displaySimple.setChar(0, ' ');
  else
    displaySimple.setChar(0, hour1);

  displaySimple.setChar(1, hour % 10);
  displaySimple.setChar(2, min / 10);
  displaySimple.setChar(3, min % 10);

  displaySimple.update();
}

DisplayRaw DisplaySimple::getRaw()
{
  return digits[0];
}

DisplaySimple displaySimple = DisplaySimple();
