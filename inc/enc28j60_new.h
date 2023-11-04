#ifndef ENC28J60_H
#define ENC28J60_H

#include "hardware/spi.h"

constexpr uint8_t read_control_register = 0x00;
constexpr uint8_t read_buffer_memory = 0x3A;
constexpr uint8_t write_control_register = 0x40;
constexpr uint8_t write_buffer_memory = 0x7A;
constexpr uint8_t bit_field_set = 0x80;
constexpr uint8_t bit_field_clear = 0xA0;
constexpr uint8_t system_reset = 0xFF;

class Enc28j60
{
private:
    spi_inst_t *mSpiInst;
    uint8_t mCsPin;

    uint8_t bank;
    uint16_t NextPacketPtr;

    void cs_enable();
    void cs_disable();

    void spi_write_single(uint8_t data);

public:
    Enc28j60(spi_inst_t *spiInst)
        : mSpiInst(spiInst)
    {}

    uint8_t read_op(uint8_t op, uint8_t address);
    void write_op(uint8_t op, uint8_t address, uint8_t data);
    void read_buffer(uint16_t len, uint8_t *data);
    void write_buffer(uint16_t len, uint8_t *data);
    void set_bank(uint8_t address);
    uint8_t read(uint8_t address, uint8_t _bank);
    void write(uint8_t address, uint8_t _bank, uint8_t data);
    void phy_write(uint8_t address, uint16_t data);
    void clkout(uint8_t clk);
    void init(uint8_t *macaddr);
    void packet_send(uint16_t len, uint8_t *packet);
    uint16_t packet_receive(uint16_t maxlen, uint8_t *packet);
    uint8_t revision();
};

#endif // ENC28J60_H

