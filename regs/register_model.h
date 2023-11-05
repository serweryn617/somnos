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

#ifndef DRAL_REGISTER_MODEL_H
#define DRAL_REGISTER_MODEL_H

#include <cstdint>

namespace dral {

template<typename T, T position, T width = 1>
class BitFieldModel
{
public:
  static constexpr T Width = width;
  static constexpr T Mask = (1U << width) - 1U;
  static constexpr T Position = position;

public:
  template<typename U>
  BitFieldModel& operator=(U value)
  {
    m_value = (m_value & ~(Mask << position)) | ((value & Mask) << position);
    return *this;
  }

  operator T() const
  {
    return (m_value >> position) & Mask;
  }

  explicit operator bool() const
  {
    return m_value & (Mask << position);
  }

  BitFieldModel& operator++()
  {
    return *this = *this + 1U;
  }

  T operator++(int)
  {
    const T result = *this;
    ++*this;
    return result;
  }

  BitFieldModel& operator--()
  {
    return *this = *this - 1U;
  }

  T operator--(int)
  {
    const T result = *this;
    --*this;
    return result;
  }

private:
  T m_value;

  static_assert(position >= 0 && position < sizeof(T) * 8, "BitFiled position invalid");
  static_assert(width >= 1 && width <= sizeof(T) * 8, "BitFiled width invalid");
};

// template<typename T, T position>
// class BitFieldModel<position, 1U>
// {
// public:
//   static constexpr T Width = 1U;
//   static constexpr T Mask = (1U << Width) - 1U;
//   static constexpr T Position = position;

// public:
//   BitFieldModel& operator=(bool value)
//   {
//     m_value = (m_value & ~(Mask << position)) | (value << position);
//     return *this;
//   }

//   explicit operator bool() const
//   {
//     return m_value & (Mask << position);
//   }

// private:
//   T m_value;

//   static_assert(position >= 0 && position <= 15, "BitFiled position must be between 0 and 15");
// };

}  // namespace

#endif /* DRAL_REGISTER_MODEL_H */
