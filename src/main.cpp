#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "ina219/base.h"
#include "i2c_device.h"


int main() {
    // Enable UART so we can print status output
    stdio_init_all();

    // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    const uint mosfet_pin = 15;
    gpio_init(mosfet_pin);
    gpio_set_dir(mosfet_pin, GPIO_OUT);
    bool sw = false;

    I2CDevice<uint8_t, uint16_t> ina219(0x40, i2c_default);

    dral::ina219::cal ina_calibration;
    ina_calibration.value = 13422;
    ina219.write(ina_calibration.Address, ina_calibration.value);

    while (true) {
        sw = !sw;
        gpio_put(mosfet_pin, sw);
        sleep_ms(1000);

        int16_t shunt_voltage = ina219.read(dral::ina219::svr::Address);
        float sh = (float)shunt_voltage / 100;
        printf("Shunt voltage: %.2f mV\n", sh);

        dral::ina219::bvr bus_voltage;
        bus_voltage.value = ina219.read(dral::ina219::bvr::Address);
        float volt = (float)bus_voltage.bd / 250;
        printf("Bus voltage: %.2f V\n", volt);

        int16_t current = ina219.read(dral::ina219::crt::Address);
        float curr = (float)current * 0.0305;
        printf("current %.2f mA\n\n", curr);

        sleep_ms(1000);
    }

    return 0;
}
