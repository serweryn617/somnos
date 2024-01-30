/*
 * D-RAL - Device Register Access Layer
 * https://github.com/gembcior/d-ral
 *
 * MIT License
 *
 * Copyright (c) 2024 Gembcior
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

#ifndef DRAL_ENC28J60_PHY_H
#define DRAL_ENC28J60_PHY_H

#include "../register_model.h"

namespace dral::enc28j60 {

struct phy
{
  struct con1
  {
    constexpr static uint8_t Address = 0x00;
    union
    {
      uint16_t value;
      BitFieldModel<uint16_t, 15, 1> prst;
      BitFieldModel<uint16_t, 14, 1> ploopbk;
      BitFieldModel<uint16_t, 11, 1> ppwrsv;
      BitFieldModel<uint16_t, 8, 1> pdpxmd;
    };
  };

  struct stat1
  {
    constexpr static uint8_t Address = 0x01;
    union
    {
      uint16_t value;
      BitFieldModel<uint16_t, 12, 1> pfdpx;
      BitFieldModel<uint16_t, 11, 1> phdpx;
      BitFieldModel<uint16_t, 2, 1> llstat;
      BitFieldModel<uint16_t, 1, 1> jbstat;
    };
  };

  struct id1
  {
    constexpr static uint8_t Address = 0x02;
    union
    {
      uint16_t value;
      BitFieldModel<uint16_t, 0, 16> oui;
    };
  };

  struct id2
  {
    constexpr static uint8_t Address = 0x03;
    union
    {
      uint16_t value;
      BitFieldModel<uint16_t, 10, 6> oui;
      BitFieldModel<uint16_t, 4, 6> ppn;
      BitFieldModel<uint16_t, 0, 4> prev;
    };
  };

  struct con2
  {
    constexpr static uint8_t Address = 0x10;
    union
    {
      uint16_t value;
      BitFieldModel<uint16_t, 14, 1> frclnk;
      BitFieldModel<uint16_t, 13, 1> txdis;
      BitFieldModel<uint16_t, 10, 1> jabber;
      BitFieldModel<uint16_t, 8, 1> hdldis;
    };
  };

  struct stat2
  {
    constexpr static uint8_t Address = 0x11;
    union
    {
      uint16_t value;
      BitFieldModel<uint16_t, 13, 1> txstat;
      BitFieldModel<uint16_t, 12, 1> rxstat;
      BitFieldModel<uint16_t, 11, 1> colstat;
      BitFieldModel<uint16_t, 10, 1> lstat;
      BitFieldModel<uint16_t, 9, 1> dpxstat;
      BitFieldModel<uint16_t, 5, 1> plrity;
    };
  };

  struct ie
  {
    constexpr static uint8_t Address = 0x12;
    union
    {
      uint16_t value;
      BitFieldModel<uint16_t, 4, 1> plnkie;
      BitFieldModel<uint16_t, 1, 1> pgeie;
    };
  };

  struct ir
  {
    constexpr static uint8_t Address = 0x13;
    union
    {
      uint16_t value;
      BitFieldModel<uint16_t, 4, 1> plnkif;
      BitFieldModel<uint16_t, 2, 1> pgif;
    };
  };

  struct lcon
  {
    constexpr static uint8_t Address = 0x14;
    union
    {
      uint16_t value;
      BitFieldModel<uint16_t, 8, 4> lacfg;
      BitFieldModel<uint16_t, 4, 4> lbcfg;
      BitFieldModel<uint16_t, 2, 2> lfrq;
      BitFieldModel<uint16_t, 1, 1> strch;
    };
  };

};

}

#endif /* DRAL_ENC28J60_PHY_H */