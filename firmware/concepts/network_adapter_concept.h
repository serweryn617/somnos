#ifndef NETWORK_ADAPTER_CONCEPT_H
#define NETWORK_ADAPTER_CONCEPT_H

#include <concepts>
#include <cstdint>

namespace concepts::network {

// clang-format off

template<typename T>
concept network_adapter_concept = requires(T adapter, uint16_t len, uint8_t* packet) {
    { adapter.packet_send(len, packet) } -> std::same_as<void>;
    { adapter.packet_receive(len, packet) } -> std::integral;
};

// clang-format on

}  // namespace concepts::network

#endif  // NETWORK_ADAPTER_CONCEPT_H