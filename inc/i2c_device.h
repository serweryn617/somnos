#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include <concepts>
#include "hardware/i2c.h"

template<typename _I2CAddr, typename _RegVal, typename _RegAddr = _I2CAddr>
    requires std::integral<_I2CAddr> && std::integral<_RegVal> && std::integral<_RegAddr>
class I2CDevice  // TODO: Is generic I2C device class needed?
{
private:
    _I2CAddr mI2CAddress;
    i2c_inst_t *mI2CInst;

public:
    I2CDevice(_I2CAddr i2cAddress, i2c_inst_t *i2cInst)
        : mI2CAddress(i2cAddress)
        , mI2CInst(i2cInst)
    {}

    _RegVal read(_RegAddr regAddress)
    {
        uint8_t data[2];

        i2c_write_blocking(mI2CInst, mI2CAddress, &regAddress, 1, false);
        i2c_read_blocking(mI2CInst, mI2CAddress, data, 2, false);

        return (data[0] << 8) | (data[1]);
    }

    void write(_RegAddr regAddress, _RegVal value, uint8_t num_bytes = 3)  // TODO: Fix num_bytes
    {
        uint8_t data[3];
        data[0] = regAddress;
        data[1] = value >> 8;
        data[2] = value & 0xff;

        i2c_write_blocking(mI2CInst, mI2CAddress, data, num_bytes, false);
    }
};

#endif // I2C_DEVICE_H