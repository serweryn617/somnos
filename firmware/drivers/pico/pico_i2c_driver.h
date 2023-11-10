#ifndef PICO_I2C_DRIVER_H
#define PICO_I2C_DRIVER_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "drivers/concepts/i2c_driver_concept.h"

namespace drivers::pico {

class PicoI2CDriver
{
private:
    i2c_inst_t *i2c_inst_;
    uint8_t i2c_address_;
    uint8_t pin_sda_, pin_scl_;

public:
    PicoI2CDriver(i2c_inst_t *i2c_inst, uint8_t pin_sda, uint8_t pin_scl, uint8_t i2c_address)
        : i2c_inst_(i2c_inst)
        , i2c_address_(i2c_address)
        , pin_sda_(pin_sda)
        , pin_scl_(pin_scl)
    {};

    void init()
    {
        i2c_init(i2c_inst_, 100 * 1000);
        gpio_set_function(pin_sda_, GPIO_FUNC_I2C);
        gpio_set_function(pin_scl_, GPIO_FUNC_I2C);
        gpio_pull_up(pin_sda_);
        gpio_pull_up(pin_scl_);
    }

    int read_data(uint8_t *buffer, uint8_t length = 1)
    {
        return i2c_read_blocking(i2c_inst_, i2c_address_, buffer, length, false);
    }

    int write_data(uint8_t *buffer, uint8_t length = 1)
    {
        return i2c_write_blocking(i2c_inst_, i2c_address_, buffer, length, false);
    }
};

static_assert(drivers::concepts::i2c_driver_concept<PicoI2CDriver>);

}  // namespace drivers::pico

#endif  // PICO_I2C_DRIVER_H