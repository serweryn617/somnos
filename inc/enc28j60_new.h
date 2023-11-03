#ifndef ENC28J60_H
#define ENC28J60_H

#include "hardware/spi.h"

class Enc28j60
{
private:
    spi_inst_t *mSpiInst;

public:
    Enc28j60(spi_inst_t *spiInst)
        : mSpiInst(spiInst)
    {}

    uint8_t read_op(uint8_t op, uint8_t address);
    void write_op(uint8_t op, uint8_t address, uint8_t data);
    void read_buffer(uint16_t len, uint8_t *data);
    void write_buffer(uint16_t len, uint8_t *data);
    void set_bank(uint8_t address);
    uint8_t read(uint8_t address);
    void write(uint8_t address, uint8_t data);
    void phy_write(uint8_t address, uint16_t data);
    void clkout(uint8_t clk);
    void init(uint8_t *macaddr);
    void packet_send(uint16_t len, uint8_t *packet);
    uint16_t packet_receive(uint16_t maxlen, uint8_t *packet);
    uint8_t revision();
};

#endif // ENC28J60_H