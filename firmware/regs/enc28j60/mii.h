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

#ifndef DRAL_ENC28J60_MII_H
#define DRAL_ENC28J60_MII_H

#include "../register_model.h"

namespace dral::enc28j60 {

struct mii
{
  struct cmd
  {
    constexpr static uint8_t Address = 0x12;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 1, 1> scan;
      BitFieldModel<uint8_t, 0, 1> rd;
    };
  };

  struct regadr
  {
    constexpr static uint8_t Address = 0x14;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct wrl
  {
    constexpr static uint8_t Address = 0x16;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
    };
  };

  struct wrh
  {
    constexpr static uint8_t Address = 0x17;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
    };
  };

  struct rdl
  {
    constexpr static uint8_t Address = 0x18;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
    };
  };

  struct rdh
  {
    constexpr static uint8_t Address = 0x19;
    constexpr static uint8_t RegBank = 0x02;
    union
    {
      uint8_t value;
    };
  };

  struct stat
  {
    constexpr static uint8_t Address = 0x0A;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 2, 1> nvalid;
      BitFieldModel<uint8_t, 1, 1> scan;
      BitFieldModel<uint8_t, 0, 1> busy;
    };
  };

};

}

#endif /* DRAL_ENC28J60_MII_H */