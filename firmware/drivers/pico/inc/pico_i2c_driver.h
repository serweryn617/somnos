#ifndef PICO_I2C_DRIVER_H
#define PICO_I2C_DRIVER_H

#include "drivers/concepts/i2c_driver_concept.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

namespace drivers::pico {

class PicoI2CDriver {
private:
    i2c_inst_t* i2c_inst_;
    uint8_t i2c_address_;
    uint8_t pin_sda_, pin_scl_;

public:
    PicoI2CDriver(i2c_inst_t* i2c_inst, uint8_t pin_sda, uint8_t pin_scl, uint8_t i2c_address);

    void init();
    int read_data(uint8_t* buffer, uint8_t length = 1);
    int write_data(uint8_t* buffer, uint8_t length = 1);
};

static_assert(drivers::concepts::i2c_driver_concept<PicoI2CDriver>);

}  // namespace drivers::pico

#endif  // PICO_I2C_DRIVER_H