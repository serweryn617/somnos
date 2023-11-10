#ifndef ENC28J60_ENUMS_H
#define ENC28J60_ENUMS_H

#include <cstdint>

namespace devices::enc28j60 {

enum class cs : bool
{
    enable = 0,
    disable = 1,
};

enum class op : uint8_t
{
    read_control_register = 0x00,
    read_buffer_memory = 0x3A,
    write_control_register = 0x40,
    write_buffer_memory = 0x7A,
    bit_field_set = 0x80,
    bit_field_clear = 0xA0,
    system_reset = 0xFF,
};

}  // namespace devices::enc28j60

#endif  // ENC28J60_ENUMS_H