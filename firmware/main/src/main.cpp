#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <stdio.h>
#include <string.h>

#include "devices/enc28j60/enc28j60.h"
#include "devices/ina219/ina219.h"
#include "drivers/pico/i2c/inc/pico_i2c_driver.h"
#include "drivers/pico/spi/inc/pico_spi_driver.h"

#include "main/inc/hw.h"
#include "network_interface/network_interface.h"

using namespace devices::enc28j60;
using namespace devices::ina219;
using namespace drivers::pico;
using namespace network;

int main()
{
    stdio_init_all();

    // Make the I2C pins available to picotool
    // bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    const uint mosfet_pin = 18;
    gpio_init(mosfet_pin);
    gpio_set_dir(mosfet_pin, GPIO_OUT);
    // bool sw = false;
    gpio_put(mosfet_pin, true);

    PicoI2CDriver i2c_driver(i2c0, hw::ina219::i2c::sda, hw::ina219::i2c::scl, hw::ina219::i2c::address);
    i2c_driver.init();
    INA219 ina219(i2c_driver);
    ina219.calibrate();

    uint8_t mac_addr[6] = { 0x11, 0xe8, 0xc3, 0xf8, 0xc6, 0x92 };
    PicoSPIDriver spi_driver(spi1, hw::enc28j60::spi::cs, hw::enc28j60::spi::miso, hw::enc28j60::spi::mosi, hw::enc28j60::spi::sck);
    spi_driver.init();
    Enc28j60 enc28j60(spi_driver);
    enc28j60.init(mac_addr);
    Interface network_interface(enc28j60);
    network_interface.init();

    while (true) {
        // sw = !sw;
        // sleep_ms(100);
        // gpio_put(mosfet_pin, sw);
        // sleep_ms(100);

        // float sh = ina219.shunt_voltage();
        // printf("Shunt voltage: %.2f mV\n", sh);

        // float bus = ina219.bus_voltage();
        // printf("Bus voltage: %.2f V\n", bus);

        network_interface.receive();

        uint16_t bus = ina219.bus_voltage_raw();
        uint16_t shunt = ina219.shunt_voltage_raw();
        uint16_t current = ina219.current_raw();
        uint16_t power = ina219.power_raw();

        uint8_t data[12] = {'S', 'M', 'N', 'S'};  // magic
        data[4] = bus & 0xff;
        data[5] = bus >> 8;
        data[6] = shunt & 0xff;
        data[7] = shunt >> 8;
        data[8] = current & 0xff;
        data[9] = current >> 8;
        data[10] = power & 0xff;
        data[11] = power >> 8;

        network_interface.send(data, sizeof(data));

        network_interface.check_timers();

        sleep_ms(1000);
    }

    return 0;
}
