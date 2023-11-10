#ifndef INA219_H
#define INA219_H

#include "drivers/concepts/i2c_driver_concept.h"
#include "regs/ina219/base.h"

namespace devices::ina219 {

template<drivers::concepts::i2c_driver_concept I2CDriver>
class INA219 {
private:
    I2CDriver& i2c_driver_;

public:
    INA219(I2CDriver& i2c_driver)
        : i2c_driver_(i2c_driver)
    {
    }

    uint16_t read_register(uint8_t address)
    {
        uint8_t data[2];

        i2c_driver_.write_data(&address, 1);
        i2c_driver_.read_data(data, 2);

        return (data[0] << 8) | (data[1]);
    }

    void write_register(uint8_t address, uint16_t value)
    {
        uint8_t data[3];

        data[0] = address;
        data[1] = value >> 8;
        data[2] = value & 0xff;

        i2c_driver_.write_data(data, 3);
    }

    void calibrate()
    {
        dral::ina219::cal ina_calibration;
        ina_calibration.value = 13422;
        write_register(ina_calibration.Address, ina_calibration.value);
    }

    uint16_t shunt_voltage_raw() { return read_register(dral::ina219::shunt::Address); }

    float shunt_voltage()
    {
        uint16_t shunt_voltage = shunt_voltage_raw();
        return static_cast<float>(shunt_voltage) / 100;
    }

    uint16_t bus_voltage_raw()
    {
        dral::ina219::bus bus;
        bus.value = read_register(dral::ina219::bus::Address);
        return bus.data;
    }

    float bus_voltage()
    {
        uint16_t shunt_voltage = shunt_voltage_raw();
        return static_cast<float>(shunt_voltage) / 250;
    }

    // uint16_t power_raw()
    // {
    //     return read_register(dral::ina219::pwr::Address);
    // }

    // float power()
    // {
    //     uint16_t shunt_voltage = shunt_voltage_raw();
    //     return static_cast<float>(shunt_voltage) / 100;
    // }

    uint16_t current_raw() { return read_register(dral::ina219::curr::Address); }

    float current()
    {
        uint16_t shunt_voltage = shunt_voltage_raw();
        return static_cast<float>(shunt_voltage) * 0.0305;
    }
};

}  // namespace devices::ina219

#endif  // INA219_H