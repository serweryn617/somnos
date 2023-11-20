#include "drivers/pico/i2c/inc/pico_i2c_driver.h"

namespace drivers::pico {

PicoI2CDriver::PicoI2CDriver(i2c_inst_t* i2c_inst, uint8_t pin_sda, uint8_t pin_scl, uint8_t i2c_address)
    : i2c_inst_(i2c_inst)
    , i2c_address_(i2c_address)
    , pin_sda_(pin_sda)
    , pin_scl_(pin_scl){};

void PicoI2CDriver::init()
{
    i2c_init(i2c_inst_, 100 * 1000);
    gpio_set_function(pin_sda_, GPIO_FUNC_I2C);
    gpio_set_function(pin_scl_, GPIO_FUNC_I2C);
    gpio_pull_up(pin_sda_);
    gpio_pull_up(pin_scl_);
}

int PicoI2CDriver::read_data(uint8_t* buffer, uint8_t length)
{
    return i2c_read_blocking(i2c_inst_, i2c_address_, buffer, length, false);
}

int PicoI2CDriver::write_data(const uint8_t* buffer, uint8_t length)
{
    return i2c_write_blocking(i2c_inst_, i2c_address_, buffer, length, false);
}

}  // namespace drivers::pico
