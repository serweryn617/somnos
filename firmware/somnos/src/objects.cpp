#include "devices/enc28j60/enc28j60.h"
#include "devices/ina219/ina219.h"
#include "drivers/pico/inc/pico_i2c_driver.h"
#include "drivers/pico/inc/pico_spi_driver.h"
#include "somnos/inc/hw.h"
#include "somnos/inc/objects.h"

using namespace devices::enc28j60;
using namespace devices::ina219;
using namespace drivers::pico;

namespace objects {

PicoSPIDriver enc_driver(spi0, hw::enc28j60::spi::cs, hw::enc28j60::spi::miso, hw::enc28j60::spi::mosi, hw::enc28j60::spi::sck);
Enc28j60<PicoSPIDriver> enc28j60(enc_driver);

}