#ifndef HW_H
#define HW_H

#include <cstdint>
#include "hardware/i2c.h"
#include "hardware/spi.h"

namespace hw {

struct ina219 {
    struct i2c {
        // static constexpr i2c_inst_t* port = i2c0;
        static constexpr uint8_t sda = 16;
        static constexpr uint8_t scl = 17;
        static constexpr uint8_t address = 0x40;
    };
};

struct enc28j60 {
    struct spi {
        // static constexpr spi_inst_t* port = spi1;
        static constexpr uint8_t sck = 10;
        static constexpr uint8_t mosi = 11;
        static constexpr uint8_t miso = 12;
        static constexpr uint8_t cs = 13;
    };
};

struct netif {
    static constexpr uint8_t mac_addr[6] = { 0x11, 0xe8, 0xc3, 0xf8, 0xc6, 0x92 };
    static constexpr uint32_t ethernet_mtu = 1500;
};

}

#endif  // HW_H