#ifndef SPI_DRIVER_CONCEPT_H
#define SPI_DRIVER_CONCEPT_H

#include <concepts>

namespace drivers::concepts {

template<typename T>
concept spi_driver_concept = requires(T spi_driver, uint8_t *buffer, uint8_t length, bool state) {
    { spi_driver.read_data(buffer, length) } -> std::integral;
    { spi_driver.write_data(buffer, length) } -> std::integral;
    { spi_driver.read_data(buffer) } -> std::integral;
    { spi_driver.write_data(buffer) } -> std::integral;
    { spi_driver.set_cs(state) } -> std::same_as<void>;
};

}  // namespace drivers::concepts

#endif  // SPI_DRIVER_CONCEPT_H