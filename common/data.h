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
 * data.h
 *
 * Created: 9/27/2019 12:49:11 PM
 *  Author: juneh
 */


#ifndef DATA_H_
#define DATA_H_

#include "types.h"

// Get the pointer to a property of a port
inline volatile __attribute__ ((const, hot)) var32* ptr32(var32 addr)
{
  return (volatile var32*)(addr);
}

inline volatile __attribute__ ((const, hot)) var16* ptr16(var32 addr)
{
  return (volatile var16*)(addr);
}

inline volatile __attribute__ ((const, hot)) var8* ptr8(var32 addr)
{
  return (volatile var8*)(addr);
}

inline volatile __attribute__ ((const, cold)) svar32* sptr32(var32 addr)
{
  return (volatile svar32*)(addr);
}

inline volatile __attribute__ ((const, cold)) svar16* sptr16(var32 addr)
{
  return (volatile svar16*)(addr);
}

inline volatile __attribute__ ((const, cold)) svar8* sptr8(var32 addr)
{
  return (volatile svar8*)(addr);
}

// Write an arbitrary value to a property of a port
inline void __attribute__ ((hot)) set32(var32 addr, var32 val = 0, volatile var32* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = ptr32(addr);

  *ptr = val;
}

inline void __attribute__ ((cold)) sset32(var32 addr, svar32 val = 0, volatile svar32* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = sptr32(addr);

  *ptr = val;
}

// Retrieve an arbitrary value from a property of a port
inline var32 __attribute__ ((hot)) get32(var32 addr, volatile var32* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = ptr32(addr);

  return *ptr;
}

inline svar32 __attribute__ ((cold)) sget32(var32 addr, volatile svar32* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = sptr32(addr);

  return *ptr;
}

inline void __attribute__ ((hot)) set16(var32 addr, var32 val = 0, volatile var16* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = ptr16(addr);

  *ptr = val;
}

inline void __attribute__ ((cold)) sset16(var32 addr, svar32 val = 0, volatile svar16* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = sptr16(addr);

  *ptr = val;
}

// Retrieve an arbitrary value from a property of a port
inline var16 __attribute__ ((hot)) get16(var32 addr, volatile var16* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = ptr16(addr);

  return *ptr;
}

inline svar16 __attribute__ ((cold)) sget16(var32 addr, volatile svar16* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = sptr16(addr);

  return *ptr;
}

inline void __attribute__ ((hot)) set8(var32 addr, var8 val = 0, volatile var8* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = ptr8(addr);

  *ptr = val;
}

inline void __attribute__ ((cold)) sset8(var32 addr, svar8 val = 0, volatile svar8* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = sptr8(addr);

  *ptr = val;
}

// Retrieve an arbitrary value from a property of a port
inline var8 __attribute__ ((hot)) get8(var32 addr, volatile var8* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = ptr8(addr);

  return *ptr;
}

inline svar8 __attribute__ ((cold)) sget8(var32 addr, volatile svar8* ptr = nullptr)
{
  if(ptr == nullptr)
  ptr = sptr8(addr);

  return *ptr;
}

// val1 = LSB
// val2 = MSB
constexpr var16 __attribute__ ((const, hot)) concat8to16(var8 val1 = 0, var8 val2 = 0)
{
  return (val2 << 8) | val1;
}

constexpr svar16 __attribute__ ((const, cold)) sconcat8to16(svar8 val1 = 0, svar8 val2 = 0)
{
  return (val2 << 8) | val1;
}

// val1 = LSB
// val2 = MSB
constexpr var32 __attribute__ ((const, hot)) concat16to32(var16 val1 = 0, var16 val2 = 0)
{
  return (val2 << 16) | val1;
}

constexpr svar32 __attribute__ ((const, cold)) sconcat16to32(svar16 val1 = 0, svar16 val2 = 0)
{
  return (val2 << 16) | val1;
}

// val1 = LSB
// val4 = MSB
constexpr var32 __attribute__ ((const, hot)) concat8to32(var8 val1 = 0, var8 val2 = 0, var8 val3 = 0, var8 val4 = 0)
{
  return (val4 << 24) | (val3 << 16) | (val2 << 8) | val1;
}

constexpr svar32 __attribute__ ((const, cold)) sconcat8to32(svar8 val1 = 0, svar8 val2 = 0, svar8 val3 = 0, svar8 val4 = 0)
{
  return (val4 << 24) | (val3 << 16) | (val2 << 8) | val1;
}

#endif /* DATA_H_ */
