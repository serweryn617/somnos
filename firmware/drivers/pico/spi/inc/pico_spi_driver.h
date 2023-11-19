#ifndef PICO_SPI_DRIVER_H
#define PICO_SPI_DRIVER_H

#include "concepts/spi_driver_concept.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

namespace drivers::pico {

class PicoSPIDriver {
private:
    spi_inst_t* spi_inst_;
    uint8_t pin_cs_, pin_rx_, pin_tx_, pin_sck_;

public:
    PicoSPIDriver(spi_inst_t* spi_inst, uint8_t pin_cs, uint8_t pin_rx, uint8_t pin_tx, uint8_t pin_sck);

    void init();
    void set_cs(bool state);
    int read_data(uint8_t* buffer, uint8_t length = 1);
    int write_data(uint8_t* buffer, uint8_t length = 1);
};

static_assert(concepts::drivers::spi_driver_concept<PicoSPIDriver>);

}  // namespace drivers::pico

#endif  // PICO_SPI_DRIVER_H