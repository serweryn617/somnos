#ifndef NETIF_CONCEPT_H
#define NETIF_CONCEPT_H

#include <concepts>
#include <cstdint>

namespace network::concepts {

// clang-format off

template<typename T>
concept netif_concept = requires(T netif, uint16_t len, uint8_t* packet) {
    { netif.packet_send(len, packet) } -> std::same_as<void>;
    { netif.packet_receive(len, packet) } -> std::integral;
};

// clang-format on

}  // namespace drivers::concepts

#endif  // NETIF_CONCEPT_H