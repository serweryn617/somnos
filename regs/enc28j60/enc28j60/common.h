/*
 * D-RAL - Device Register Access Layer
 * https://github.com/gembcior/d-ral
 *
 * MIT License
 *
 * Copyright (c) 2023 Gembcior
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This is an auto generated file. Do not modify!
 */

#ifndef DRAL_ENC28J60_COMMON_H
#define DRAL_ENC28J60_COMMON_H

#include "../register_model.h"

namespace dral::enc28j60 {

struct ie
{
  constexpr static uint8_t Address = 0x1B;
  constexpr static uint8_t RegBank = 0x00;
  union
  {
    uint8_t value;
    BitFieldModel<7, 1> intie;
    BitFieldModel<6, 1> pktie;
    BitFieldModel<5, 1> dmaie;
    BitFieldModel<4, 1> linkie;
    BitFieldModel<3, 1> txie;
    BitFieldModel<1, 1> txerie;
    BitFieldModel<0, 1> rxerie;
  };
};

struct ir
{
  constexpr static uint8_t Address = 0x1C;
  constexpr static uint8_t RegBank = 0x00;
  union
  {
    uint8_t value;
    BitFieldModel<6, 1> pktif;
    BitFieldModel<5, 1> dmaif;
    BitFieldModel<4, 1> linkif;
    BitFieldModel<3, 1> txif;
    BitFieldModel<1, 1> txerif;
    BitFieldModel<0, 1> rxerif;
  };
};

struct stat
{
  constexpr static uint8_t Address = 0x1D;
  constexpr static uint8_t RegBank = 0x00;
  union
  {
    uint8_t value;
    BitFieldModel<7, 1> int;
    BitFieldModel<6, 1> bufer;
    BitFieldModel<4, 1> latecol;
    BitFieldModel<2, 1> rxbusy;
    BitFieldModel<1, 1> txabrt;
    BitFieldModel<0, 1> clkrdy;
  };
};

struct con2
{
  constexpr static uint8_t Address = 0x1E;
  constexpr static uint8_t RegBank = 0x00;
  union
  {
    uint8_t value;
    BitFieldModel<7, 1> autoinc;
    BitFieldModel<6, 1> pktdec;
    BitFieldModel<5, 1> pwrsv;
    BitFieldModel<3, 1> vrps;
  };
};

struct con1
{
  constexpr static uint8_t Address = 0x1F;
  constexpr static uint8_t RegBank = 0x00;
  union
  {
    uint8_t value;
    BitFieldModel<7, 1> txrst;
    BitFieldModel<6, 1> rxrst;
    BitFieldModel<5, 1> dmast;
    BitFieldModel<4, 1> csumen;
    BitFieldModel<3, 1> txrts;
    BitFieldModel<2, 1> rxen;
    BitFieldModel<0, 2> bsel;
  };
};

}

#endif /* DRAL_ENC28J60_COMMON_H */
