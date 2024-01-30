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

#ifndef DRAL_ENC28J60_ETH_H
#define DRAL_ENC28J60_ETH_H

#include "../register_model.h"

namespace dral::enc28j60 {

struct eth
{
  struct rdptl
  {
    constexpr static uint8_t Address = 0x00;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct rdpth
  {
    constexpr static uint8_t Address = 0x01;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct wrptl
  {
    constexpr static uint8_t Address = 0x02;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct wrpth
  {
    constexpr static uint8_t Address = 0x03;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct txstl
  {
    constexpr static uint8_t Address = 0x04;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct txsth
  {
    constexpr static uint8_t Address = 0x05;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct txndl
  {
    constexpr static uint8_t Address = 0x06;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct txndh
  {
    constexpr static uint8_t Address = 0x07;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct rxstl
  {
    constexpr static uint8_t Address = 0x08;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct rxsth
  {
    constexpr static uint8_t Address = 0x09;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct rxndl
  {
    constexpr static uint8_t Address = 0x0A;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct rxndh
  {
    constexpr static uint8_t Address = 0x0B;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct rxrdptl
  {
    constexpr static uint8_t Address = 0x0C;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct rxrdpth
  {
    constexpr static uint8_t Address = 0x0D;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct rxwrptl
  {
    constexpr static uint8_t Address = 0x0E;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct rxwrpth
  {
    constexpr static uint8_t Address = 0x0F;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct dmastl
  {
    constexpr static uint8_t Address = 0x10;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct dmasth
  {
    constexpr static uint8_t Address = 0x11;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct dmandl
  {
    constexpr static uint8_t Address = 0x12;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct dmandh
  {
    constexpr static uint8_t Address = 0x13;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct dmadstl
  {
    constexpr static uint8_t Address = 0x14;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct dmadsth
  {
    constexpr static uint8_t Address = 0x15;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct dmacsl
  {
    constexpr static uint8_t Address = 0x16;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct dmacsh
  {
    constexpr static uint8_t Address = 0x17;
    constexpr static uint8_t RegBank = 0x00;
    union
    {
      uint8_t value;
    };
  };

  struct ht0
  {
    constexpr static uint8_t Address = 0x00;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct ht1
  {
    constexpr static uint8_t Address = 0x01;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct ht2
  {
    constexpr static uint8_t Address = 0x02;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct ht3
  {
    constexpr static uint8_t Address = 0x03;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct ht4
  {
    constexpr static uint8_t Address = 0x04;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct ht5
  {
    constexpr static uint8_t Address = 0x05;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct ht6
  {
    constexpr static uint8_t Address = 0x06;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct ht7
  {
    constexpr static uint8_t Address = 0x07;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmm0
  {
    constexpr static uint8_t Address = 0x08;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmm1
  {
    constexpr static uint8_t Address = 0x09;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmm2
  {
    constexpr static uint8_t Address = 0x0A;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmm3
  {
    constexpr static uint8_t Address = 0x0B;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmm4
  {
    constexpr static uint8_t Address = 0x0C;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmm5
  {
    constexpr static uint8_t Address = 0x0D;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmm6
  {
    constexpr static uint8_t Address = 0x0E;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmm7
  {
    constexpr static uint8_t Address = 0x0F;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmcsl
  {
    constexpr static uint8_t Address = 0x10;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmcsh
  {
    constexpr static uint8_t Address = 0x11;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmol
  {
    constexpr static uint8_t Address = 0x14;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct pmoh
  {
    constexpr static uint8_t Address = 0x15;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct rxfcon
  {
    constexpr static uint8_t Address = 0x18;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 7, 1> ucen;
      BitFieldModel<uint8_t, 6, 1> andor;
      BitFieldModel<uint8_t, 5, 1> crcen;
      BitFieldModel<uint8_t, 4, 1> pmen;
      BitFieldModel<uint8_t, 3, 1> mpen;
      BitFieldModel<uint8_t, 2, 1> hten;
      BitFieldModel<uint8_t, 1, 1> mcen;
      BitFieldModel<uint8_t, 0, 1> bcen;
    };
  };

  struct pktcnt
  {
    constexpr static uint8_t Address = 0x19;
    constexpr static uint8_t RegBank = 0x01;
    union
    {
      uint8_t value;
    };
  };

  struct bstsd
  {
    constexpr static uint8_t Address = 0x06;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

  struct bstcon
  {
    constexpr static uint8_t Address = 0x07;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 5, 3> psv;
      BitFieldModel<uint8_t, 4, 1> psel;
      BitFieldModel<uint8_t, 2, 2> tmsel;
      BitFieldModel<uint8_t, 1, 1> tme;
      BitFieldModel<uint8_t, 0, 1> bistst;
    };
  };

  struct bstcsl
  {
    constexpr static uint8_t Address = 0x08;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

  struct bstcsh
  {
    constexpr static uint8_t Address = 0x09;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

  struct revid
  {
    constexpr static uint8_t Address = 0x12;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 5> data;
    };
  };

  struct cocon
  {
    constexpr static uint8_t Address = 0x15;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 0, 3> data;
    };
  };

  struct flocon
  {
    constexpr static uint8_t Address = 0x17;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
      BitFieldModel<uint8_t, 2, 1> fuldpxs;
      BitFieldModel<uint8_t, 0, 2> fcen;
    };
  };

  struct pausl
  {
    constexpr static uint8_t Address = 0x18;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

  struct paush
  {
    constexpr static uint8_t Address = 0x19;
    constexpr static uint8_t RegBank = 0x03;
    union
    {
      uint8_t value;
    };
  };

};

}

#endif /* DRAL_ENC28J60_ETH_H */