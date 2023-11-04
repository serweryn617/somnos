#ifndef PICO_SPI_DEVICE_H
#define PICO_SPI_DEVICE_H

#include "pico/stdlib.h"

class PicoSPIDevice
{
private:
    spi_inst_t *spi_inst_;
    uint8_t pin_cs_, pin_rx_, pin_tx_, pin_sck_;

public:
    PicoSPIDevice(spi_inst_t *spi_inst, uint8_t pin_cs, uint8_t pin_rx, uint8_t pin_tx, uint8_t pin_sck)
        : spi_inst_(spi_inst)
        , pin_cs_(pin_cs)
        , pin_rx_(pin_rx)
        , pin_tx_(pin_tx)
        , pin_sck_(pin_sck)
    {};

    int read_data(uint8_t *buffer, uint8_t length = 1)
    {
        return spi_read_blocking(spi_inst_, 0, buffer, length);
    }

    int write_data(uint8_t *buffer, uint8_t length = 1)
    {
        return spi_write_blocking(spi_inst_, buffer, length);
    }
};

#endif // PICO_SPI_DEVICE_H