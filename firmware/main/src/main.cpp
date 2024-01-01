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


struct payload {
    char magic[4] = {'S', 'M', 'N', 'S'};
    float bus;
    float shunt;
    float current;
    float power;
};


int main()
{
    stdio_init_all();

    const uint mosfet_pin = 18;
    gpio_init(mosfet_pin);
    gpio_set_dir(mosfet_pin, GPIO_OUT);
    gpio_put(mosfet_pin, true);

    PicoI2CDriver i2c_driver(i2c0, hw::ina219::i2c::sda, hw::ina219::i2c::scl, hw::ina219::i2c::address);
    i2c_driver.init();
    INA219 ina219(i2c_driver);
    ina219.calibrate();

    uint8_t mac_addr[6] = { 0x11, 0xe8, 0xc3, 0xf8, 0xc6, 0x92 };  // TODO: remove
    PicoSPIDriver spi_driver(spi1, hw::enc28j60::spi::cs, hw::enc28j60::spi::miso, hw::enc28j60::spi::mosi, hw::enc28j60::spi::sck);
    spi_driver.init();
    Enc28j60 enc28j60(spi_driver);
    enc28j60.init(mac_addr);
    Interface network_interface(enc28j60);
    network_interface.init();

    uint8_t udp_rcv_data[10] = {0};
    network_interface.set_udp_rcv_buffer(udp_rcv_data, sizeof(udp_rcv_data));

    while (true) {
        network_interface.receive();

        if (uint16_t len = network_interface.get_udp_rcv_len(); len >= 5) {
            if (udp_rcv_data[0] == 'S' && udp_rcv_data[1] == 'M' && udp_rcv_data[2] == 'N' && udp_rcv_data[3] == 'S') {
                if (udp_rcv_data[4] == '0') {
                    gpio_put(mosfet_pin, false);
                }
                if (udp_rcv_data[4] == '1') {
                    gpio_put(mosfet_pin, true);
                }
                if (udp_rcv_data[4] == 'R') {
                    payload payload;
                    payload.bus = ina219.bus_voltage();
                    payload.shunt = ina219.shunt_voltage();
                    payload.current = ina219.current();
                    payload.power = ina219.power();

                    network_interface.send(reinterpret_cast<uint8_t*>(&payload), sizeof(payload));
                }
            }
        }

        network_interface.check_timers();
    }

    return 0;
}
