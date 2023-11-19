#include "drivers/pico/inc/pico_spi_driver.h"

namespace drivers::pico {

PicoSPIDriver::PicoSPIDriver(spi_inst_t* spi_inst, uint8_t pin_cs, uint8_t pin_rx, uint8_t pin_tx, uint8_t pin_sck)
    : spi_inst_(spi_inst)
    , pin_cs_(pin_cs)
    , pin_rx_(pin_rx)
    , pin_tx_(pin_tx)
    , pin_sck_(pin_sck){};

void PicoSPIDriver::init()
{
    spi_init(spi_inst_, 1 * 1000 * 1000);
    gpio_set_function(pin_cs_, GPIO_FUNC_SIO);
    gpio_set_function(pin_rx_, GPIO_FUNC_SPI);
    gpio_set_function(pin_tx_, GPIO_FUNC_SPI);
    gpio_set_function(pin_sck_, GPIO_FUNC_SPI);
    gpio_set_dir(pin_cs_, GPIO_OUT);
    gpio_put(pin_cs_, 1);
}

void PicoSPIDriver::set_cs(bool state)
{
    gpio_put(pin_cs_, state);
}

int PicoSPIDriver::read_data(uint8_t* buffer, uint8_t length)
{
    return spi_read_blocking(spi_inst_, 0, buffer, length);
}

int PicoSPIDriver::write_data(uint8_t* buffer, uint8_t length)
{
    return spi_write_blocking(spi_inst_, buffer, length);
}

}  // namespace drivers::pico
