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
