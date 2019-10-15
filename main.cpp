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

#include "sam.h"
#include "./boot.h"
#include "./common/types.h"
#include "./devices/display/DisplaySimple.h"
#include "./components/nvm/nvm.h"

extern void loop();

int __attribute__((cold)) main(void)
{
  // Boot device and Init Display Charmap
  Boot::boot();
  DisplayRaw::initCharMap();

  // Enable Auto Colon & Time Tick
  displaySimple.autoTickEn = true;
  displaySimple.autoClockEn = true;

  // Do initial clock update
  displaySimple.doClockUpdate();

  loop();

  return 0;
}
