#ifndef OBJECTS_H
#define OBJECTS_H

#include "devices/enc28j60/enc28j60.h"
#include "devices/ina219/ina219.h"
#include "drivers/pico/inc/pico_i2c_driver.h"
#include "drivers/pico/inc/pico_spi_driver.h"

namespace objects {

extern drivers::pico::PicoSPIDriver enc_driver;
extern devices::enc28j60::Enc28j60<drivers::pico::PicoSPIDriver> enc28j60;

}  // namespace objects

#endif  // OBJECTS_H