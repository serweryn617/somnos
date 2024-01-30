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

#ifndef DRAL_ENC28J60_MAC_H
#define DRAL_ENC28J60_MAC_H

#include "../register_model.h"

namespace dral::enc28j60 {

struct mac
{
  struct con1
  {
    constexpr static uint8_t Address = 0x00;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 3, 1> txpaus;
      BitFieldModel<uint8_t, 2, 1> rxpaus;
      BitFieldModel<uint8_t, 1, 1> passall;
      BitFieldModel<uint8_t, 0, 1> rxen;
    };
  };

  struct con3
  {
    constexpr static uint8_t Address = 0x02;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 5, 3> padcfg;
      BitFieldModel<uint8_t, 4, 1> txcrcen;
      BitFieldModel<uint8_t, 3, 1> phdren;
      BitFieldModel<uint8_t, 2, 1> hfrmen;
      BitFieldModel<uint8_t, 1, 1> frmlnen;
      BitFieldModel<uint8_t, 0, 1> fuldpx;
    };
  };

  struct con4
  {
    constexpr static uint8_t Address = 0x03;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 6, 1> defer;
      BitFieldModel<uint8_t, 5, 1> bpen;
      BitFieldModel<uint8_t, 4, 1> nobkoff;
    };
  };

  struct bbipg
  {
    constexpr static uint8_t Address = 0x04;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 7> data;
    };
  };

  struct ipgl
  {
    constexpr static uint8_t Address = 0x06;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 7> data;
    };
  };

  struct ipgh
  {
    constexpr static uint8_t Address = 0x07;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 7> data;
    };
  };

  struct clcon1
  {
    constexpr static uint8_t Address = 0x08;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 4> data;
    };
  };

  struct clcon2
  {
    constexpr static uint8_t Address = 0x09;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 6> data;
    };
  };

  struct mxfll
  {
    constexpr static uint8_t Address = 0x0A;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
    };
  };

  struct mxflh
  {
    constexpr static uint8_t Address = 0x0B;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
    };
  };

  struct adr5
  {
    constexpr static uint8_t Address = 0x00;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

  struct adr6
  {
    constexpr static uint8_t Address = 0x01;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

  struct adr3
  {
    constexpr static uint8_t Address = 0x02;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

  struct adr4
  {
    constexpr static uint8_t Address = 0x03;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

  struct adr1
  {
    constexpr static uint8_t Address = 0x04;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

  struct adr2
  {
    constexpr static uint8_t Address = 0x05;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

};

}

#endif /* DRAL_ENC28J60_MAC_H */