/*
* DisplayRaw.cpp
*
* Created: 10/4/2019 5:13:34 PM
* Author: juneh
*/

#include "DisplayRaw.h"
#include "sam.h"

var8 charmap[0xFF];

 DisplayRaw::DisplayRaw(bool autoUpdate /*= true*/, bool autoSend /*= false*/)
{
  reset(autoUpdate, autoSend);
}

void DisplayRaw::update(bool autoSend /*= false*/)
{
  digitsRender = digits;
  segsRender = segs;
  colonRender = colon;

  if(autoSend)
    send();
}

void DisplayRaw::send()
{
  // A0-A3 (Digits 0-3)
  _out(0, 2, !_cmp(digitsRender, 0));
  _out(0, 5, !_cmp(digitsRender, 1));
  _out(0, 6, !_cmp(digitsRender, 2));
  _out(0, 4, !_cmp(digitsRender, 3));

  // D0-D7 (Digit Segments 0-7 (A-G + DP))
  _out(0, 23, _cmp(segsRender, 0));
  _out(0, 22, _cmp(segsRender, 1));
  _out(1, 17, _cmp(segsRender, 2));
  _out(1, 16, _cmp(segsRender, 3));
  _out(1, 13, _cmp(segsRender, 4));
  _out(1, 14, _cmp(segsRender, 5));
  _out(1, 15, _cmp(segsRender, 6));
  _out(1, 12, _cmp(segsRender, 7));

  // D8 (Colon)
  _out(0, 21, colonRender);
}

void DisplayRaw::reset(bool autoUpdate /*= false*/, bool autoSend /*= false*/)
{
  digits = 0x00;
  segs = 0x00;
  colon = false;

  if(autoUpdate)
    update(autoSend);
}

void DisplayRaw::setDigit(var8 ind, bool power)
{
  if(power)
    digits |= (1 << ind);
  else
    digits &= ~(1 << ind);
}

bool DisplayRaw::getDigit(var8 ind)
{
  return !_cmp(digits, ind);
}

void DisplayRaw::setSeg(var8 ind, bool power)
{
  if(power)
    segs |= (1 << ind);
  else
    segs &= ~(1 << ind);
}

void DisplayRaw::setChar(var8 ch)
{
  segs = charmap[ch];
}

bool DisplayRaw::getSeg(var8 ind)
{
  return _cmp(segs, ind);
}

void DisplayRaw::setCol(bool power)
{
  colon = power;
}

bool DisplayRaw::getCol()
{
  return colon;
}

void DisplayRaw::modifyCharmap(var8 ch, var8 data)
{
  charmap[ch] = data;
}

void DisplayRaw::initCharMap()
{
  // Insert spaces into all charmap elements
  for(var8 i = 0; i < 0xFF; i++)
  charmap[i] = 0b00000000;

  // Define Charmap elements which can be defined
  // on a 7-segment display, the rest will be a space

  // Symbols
  charmap['!'] = 0b10000010;
  charmap['"'] = 0b00100010;
  charmap['\''] = 0b00100000;
  charmap['('] = charmap['['] = 0b00111001;
  charmap[')'] = charmap[']'] = 0b00001111;
  charmap['*'] = 0b01100011;
  charmap[','] = 0b00001100;
  charmap['-'] = 0b01000000;
  charmap['.'] = 0b10000000;
  charmap['='] = 0b01001000;
  charmap['?'] = 0b01010011;
  charmap['^'] = 0b00100011;
  charmap['_'] = 0b00001000;

  // 0-9
  charmap[0] = charmap['O'] = charmap['0'] = 0b00111111;
  charmap[1] = charmap['1'] = 0b00000110;
  charmap[2] = charmap['2'] = 0b01011011;
  charmap[3] = charmap['3'] = 0b01001111;
  charmap[4] = charmap['4'] = 0b01100110;
  charmap[5] = charmap['5'] = 0b01101101;
  charmap[6] = charmap['6'] = 0b01111101;
  charmap[7] = charmap['7'] = 0b00000111;
  charmap[8] = charmap['8'] = 0b01111111;
  charmap[9] = charmap['9'] = 0b01100111;

  // Letters (Upper & lower casing uses same config
  // with exceptions)
  charmap['A'] = charmap['a'] = 0b01110111;
  charmap['B'] = charmap['b'] = 0b01111100;
  charmap['C'] = 0b00111001;
  charmap['c'] = 0b01011000;
  charmap['D'] = charmap['d'] = 0b01011110;
  charmap['E'] = charmap['e'] = 0b01111001;
  charmap['F'] = charmap['f'] = 0b01110001;
  charmap['G'] = charmap['g'] = 0b00111101;
  charmap['H'] = 0b01110110;
  charmap['h'] = 0b01110100;
  charmap['I'] = charmap['i'] = 0b00110000;
  charmap['J'] = charmap['j'] = 0b00011110;
  charmap['L'] = charmap['l'] = 0b00111000;
  charmap['n'] = 0b01010100;
  charmap['N'] = 0b00110111;
  charmap['o'] = 0b01011100;
  charmap['P'] = charmap['p'] = 0b01110011;
  charmap['Q'] = charmap['q'] = 0b01100111;
  charmap['R'] = 0b00110011;
  charmap['r'] = 0b01010000;
  charmap['S'] = charmap['s'] = 0b01101101;
  charmap['T'] = charmap['t'] = 0b01111000;
  charmap['U'] = 0b00111110;
  charmap['u'] = 0b00011100;
  charmap['W'] = charmap['w'] = 0b00101010;
  charmap['Y'] = charmap['y'] = 0b01101110;
}

bool DisplayRaw::_cmp(var8 val, var8 bit)
{
  return (val & (1 << bit)) > 0;
}

void DisplayRaw::_out(var8 port, var32 pin, bool val)
{
  if(val)
    _outPos(port, pin);
  else
    _outNeg(port, pin);
}

void DisplayRaw::_outPos(var8 port, var32 pin)
{
  PORT->Group[port].DIRSET.bit.DIRSET = (1 << pin);
  PORT->Group[port].OUTSET.bit.OUTSET = (1 << pin);
}

void DisplayRaw::_outNeg(var8 port, var32 pin)
{
  PORT->Group[port].DIRSET.bit.DIRSET = (1 << pin);
  PORT->Group[port].OUTCLR.bit.OUTCLR = (1 << pin);
}

DisplayRaw displayRaw = DisplayRaw();
