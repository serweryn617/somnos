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

#ifndef DRAL_REGISTER_MODEL_H
#define DRAL_REGISTER_MODEL_H

#include <cstdint>
#include <type_traits>

namespace dral {

/**
 * Register Model Template
 */
template<typename SizeType, unsigned int address>
class RegisterModel
{
public:
  static constexpr unsigned int Address = address; /**< Register address */

public:
  /**
   * Read value from the register
   *
   * @return Register value
   */
  static SizeType read()
  {
    volatile SizeType* reg = reinterpret_cast<volatile SizeType*>(address);
    return *reg;
  }

  /**
   * @brief Write value to the register
   *
   * @param value Value to be written
   */
  static void write(SizeType value)
  {
    volatile SizeType* reg = reinterpret_cast<volatile SizeType*>(address);
    *reg = value;
  }
};

/**
 * Register Bank Model Template
 */
template<typename SizeType, unsigned int address, unsigned int bankOffset>
class RegisterBankModel
{
public:
  static constexpr unsigned int Address = address /**< Register address */;
  static constexpr unsigned int BankOffset = bankOffset; /**< Bank offset */

public:
  /**
   * @brief Read value from the register
   *
   * @param bank Bank number
   *
   * @return Register value
   */
  static SizeType read(unsigned int bank)
  {
    volatile SizeType* reg = reinterpret_cast<volatile SizeType*>(address + (bankOffset * bank));
    return *reg;
  }

  /**
   * @brief Write value to the register
   *
   * @param bank Bank number
   * @param value Value to be written
   */
  static void write(unsigned int bank, SizeType value)
  {
    volatile SizeType* reg = reinterpret_cast<volatile SizeType*>(address + (bankOffset * bank));
    *reg = value;
  }
};

/**
 * Field Model Template specialization for Register Bank
 */
template<typename SizeType, unsigned int address, unsigned int position, unsigned int width, unsigned int bankOffset = 0>
class FieldModel
{
public:
  static constexpr unsigned int Width = width; /**< Field width */
  static constexpr unsigned int Mask = (1U << width) - 1U; /**< Field mask relative to position */
  static constexpr unsigned int Position = position; /**< Field position */

public:
  /**
   * @brief Write value to the register's field
   *
   * @param bank Bank number
   * @param value Value to be written
   */
  static void write(unsigned int bank, SizeType value)
  {
    volatile SizeType* reg = reinterpret_cast<volatile SizeType*>(address + (bankOffset * bank));
    *reg = (*reg & ~(Mask << position)) | ((value & Mask) << position);
  }

  /**
   * @brief Read the field value from the register
   *
   * @param bank Bank number
   *
   * @return Field value
   */
  static SizeType read(unsigned int bank)
  {
    volatile SizeType* reg = reinterpret_cast<volatile SizeType*>(address + (bankOffset * bank));
    return (*reg >> position) & Mask;
  }

  static_assert(position >= 0 && position <= (sizeof(SizeType) * 8 - 1), "The position of the field can't exceed the register size or be less than 0.");
  static_assert(width >= 1 && width <= ((sizeof(SizeType) * 8) - position), "The width of the field starting from the position can't exceed the register size or be less than 1.");
};


/**
 * Field Model Template
 */
template<typename SizeType, unsigned int address, unsigned int position, unsigned int width>
class FieldModel<SizeType, address, position, width>
{
public:
  static constexpr unsigned int Width = width; /**< Field width */
  static constexpr unsigned int Mask = (1U << width) - 1U; /**< Field mask relative to position */
  static constexpr unsigned int Position = position; /**< Field position */

public:
  /**
   * @brief Write value to the register's field
   *
   * @param value Value to be written
   */
  static void write(SizeType value)
  {
    volatile SizeType* reg = reinterpret_cast<volatile SizeType*>(address);
    *reg = (*reg & ~(Mask << position)) | ((value & Mask) << position);
  }

  /**
   * @brief Read the field value from the register
   *
   * @return Field value
   */
  static SizeType read()
  {
    volatile SizeType* reg = reinterpret_cast<volatile SizeType*>(address);
    return (*reg >> position) & Mask;
  }

  static_assert(position >= 0 && position <= (sizeof(SizeType) * 8 - 1), "The position of the field can't exceed the register size or be less than 0.");
  static_assert(width >= 1 && width <= ((sizeof(SizeType) * 8) - position), "The width of the field starting from the position can't exceed the register size or be less than 1.");
};


/**
 * Bit Field Mode Template
 */
template<typename SizeType, unsigned int position, unsigned int width = 1>
class BitFieldModel
{
public:
  static constexpr unsigned int Width = width; /**< Field width */
  static constexpr unsigned int Mask = (1U << width) - 1U; /**< Field mask relative to position */
  static constexpr unsigned int Position = position; /**< Field position */

public:
  template<typename T>
  BitFieldModel& operator=(T value)
  {
    m_value = (m_value & ~(Mask << position)) | ((value & Mask) << position);
    return *this;
  }

  operator SizeType() const
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

  SizeType operator++(int)
  {
    const SizeType result = *this;
    ++*this;
    return result;
  }

  BitFieldModel& operator--()
  {
    return *this = *this - 1U;
  }

  SizeType operator--(int)
  {
    const SizeType result = *this;
    --*this;
    return result;
  }

private:
  SizeType m_value;

  static_assert(position >= 0 && position <= (sizeof(SizeType) * 8 - 1), "The position of the field can't exceed the register size or be less than 0.");
  static_assert(width >= 1 && width <= ((sizeof(SizeType) * 8) - position), "The width of the field starting from the position can't exceed the register size or be less than 1.");
};

/**
 * Bit Field Mode Template specialization for 1 bit field
 */
template<typename SizeType, unsigned int position>
class BitFieldModel<SizeType, position>
{
public:
  static constexpr unsigned int Width = 1U; /**< Field width */
  static constexpr unsigned int Mask = (1U << Width) - 1U; /**< Field mask relative to position */
  static constexpr unsigned int Position = position; /**< Field position */

public:
  BitFieldModel& operator=(bool value)
  {
    m_value = (m_value & ~(Mask << position)) | (value << position);
    return *this;
  }

  explicit operator bool() const
  {
    return m_value & (Mask << position);
  }

private:
  SizeType m_value;

  static_assert(position >= 0 && position <= (sizeof(SizeType) * 8 - 1), "The position of the field can't exceed the register size or be less than 0.");
};

}  // namespace

#endif /* DRAL_REGISTER_MODEL_H */