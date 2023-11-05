#ifndef I2C_DRIVER_CONCEPT_H
#define I2C_DRIVER_CONCEPT_H

#include <concepts>

template<typename T>
concept i2c_driver_concept = requires(T i2c_driver, uint8_t *buffer, uint8_t length) {
    { i2c_driver.read_data(buffer, length) } -> std::same_as<int>;
    { i2c_driver.write_data(buffer, length) } -> std::same_as<int>;
    { i2c_driver.read_data(buffer) } -> std::same_as<int>;
    { i2c_driver.write_data(buffer) } -> std::same_as<int>;
};

#endif // I2C_DRIVER_CONCEPT_H