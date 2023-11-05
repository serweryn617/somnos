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

#ifndef DRAL_INA219_BASE_H
#define DRAL_INA219_BASE_H

#include "../register_model.h"

namespace dral::ina219 {

struct cfg
{
  constexpr static uint8_t Address = 0x00;
  union
  {
    uint16_t value;
    BitFieldModel<uint16_t, 0, 3> mode;
    BitFieldModel<uint16_t, 3, 4> sadc;
    BitFieldModel<uint16_t, 7, 4> badc;
    BitFieldModel<uint16_t, 11, 2> pg;
    BitFieldModel<uint16_t, 13, 1> brng;
    BitFieldModel<uint16_t, 15, 1> rst;
  };
};

struct shunt
{
  constexpr static uint8_t Address = 0x01;
  union
  {
    uint16_t value;
    BitFieldModel<uint16_t, 0, 16> pga;
  };
};

struct bus
{
  constexpr static uint8_t Address = 0x02;
  union
  {
    uint16_t value;
    BitFieldModel<uint16_t, 0, 1> ovf;
    BitFieldModel<uint16_t, 1, 1> cnvr;
    BitFieldModel<uint16_t, 3, 13> data;
  };
};

struct pwr
{
  constexpr static uint8_t Address = 0x03;
  union
  {
    uint16_t value;
    
  };
};

struct curr
{
  constexpr static uint8_t Address = 0x04;
  union
  {
    uint16_t value;
    
  };
};

struct cal
{
  constexpr static uint8_t Address = 0x05;
  union
  {
    uint16_t value;
    
  };
};

}

#endif /* DRAL_INA219_BASE_H */
