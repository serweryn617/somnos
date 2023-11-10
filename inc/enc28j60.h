#ifndef ENC28J60_H
#define ENC28J60_H

#include <algorithm>

#include "enc28j60/common.h"
#include "enc28j60/eth.h"
#include "enc28j60/mac.h"
#include "enc28j60/mii.h"
#include "enc28j60/phy.h"

constexpr unsigned int receive_buffer_start = 0x0;  // errata
constexpr unsigned int receive_buffer_end = 0x1FFF - 0x0600 - 1;

constexpr unsigned int transmit_buffer_start = 0x1FFF - 0x0600;
constexpr unsigned int transmit_buffer_end = 0x1FFF;

constexpr unsigned int max_frame_len = 1500;

#include "spi_driver_concept.h"

using namespace dral::enc28j60;

constexpr uint8_t read_control_register = 0x00;
constexpr uint8_t read_buffer_memory = 0x3A;
constexpr uint8_t write_control_register = 0x40;
constexpr uint8_t write_buffer_memory = 0x7A;
constexpr uint8_t bit_field_set = 0x80;
constexpr uint8_t bit_field_clear = 0xA0;
constexpr uint8_t system_reset = 0xFF;

template<spi_driver_concept SPIDriver>
class Enc28j60
{
private:
    SPIDriver& spi_driver_;

    uint8_t bank_ = 0;
    uint16_t next_packet_ptr_;

    void set_bank(uint8_t requested_bank, uint8_t address)
    {
        bool common_regs = address >= 0x1B;
        bool bank_change = bank_ != requested_bank;
        if(!common_regs && bank_change)
        {
            spi_driver_.set_cs(0);
            common::con1 con1;
            uint8_t command = bit_field_clear | con1.Address;
            spi_driver_.write_data(&command);
            uint8_t data = con1.bsel.Mask << con1.bsel.Position;
            spi_driver_.write_data(&data);
            spi_driver_.set_cs(1);

            sleep_us(1);

            spi_driver_.set_cs(0);
            command = bit_field_set | con1.Address;
            spi_driver_.write_data(&command);
            data = (requested_bank & con1.bsel.Mask) << con1.bsel.Position;
            spi_driver_.write_data(&data);
            spi_driver_.set_cs(1);

            bank_ = requested_bank;
        }
    }

    uint8_t read_eth(uint8_t address, uint8_t bank)
    {
        set_bank(bank, address);
        spi_driver_.set_cs(0);

        uint8_t command = read_control_register | address;
        spi_driver_.write_data(&command);

        uint8_t data;
        spi_driver_.read_data(&data);

        spi_driver_.set_cs(1);
        return data;
    }

    uint8_t read_mac_mii(uint8_t address, uint8_t bank)
    {
        set_bank(bank, address);
        spi_driver_.set_cs(0);

        uint8_t command = read_control_register | address;
        spi_driver_.write_data(&command);

        uint8_t data;
        spi_driver_.read_data(&data);  // dummy byte
        spi_driver_.read_data(&data);

        spi_driver_.set_cs(1);
        return data;
    }

    void write_reg(uint8_t address, uint8_t bank, uint8_t data, uint8_t op = write_control_register)
    {
        set_bank(bank, address);
        spi_driver_.set_cs(0);

        uint8_t command = op | address;
        spi_driver_.write_data(&command);
        spi_driver_.write_data(&data);

        spi_driver_.set_cs(1);
    }

    uint16_t read_phy(uint8_t address, uint16_t data)
    {
        write_reg(mii::regadr::Address, mii::regadr::RegBank, address);

        mii::cmd cmd;
        cmd.miird = 1;
        write_reg(mii::cmd::Address, mii::cmd::RegBank, cmd.value);

        mii::stat stat;
        stat.value = read_mac_mii(mii::stat::Address, mii::stat::RegBank);
        while(stat.busy)
        {
            sleep_us(11);  // PHY read takes 10.24 us
            stat.value = read_mac_mii(mii::stat::Address, mii::stat::RegBank);
        }

        write_reg(mii::cmd::Address, mii::cmd::RegBank, 0);

        uint16_t value = 0;
        value = read_mac_mii(mii::rdl::Address, mii::rdl::RegBank);
        value |= read_mac_mii(mii::rdh::Address, mii::rdh::RegBank);
        return value;
    }

    void write_phy(uint8_t address, uint16_t data)
    {
        write_reg(mii::regadr::Address, mii::regadr::RegBank, address);

        write_reg(mii::wrl::Address, mii::wrl::RegBank, data & 0xff);
        write_reg(mii::wrh::Address, mii::wrh::RegBank, data >> 8);

        mii::stat stat;
        stat.value = read_mac_mii(mii::stat::Address, mii::stat::RegBank);
        while(stat.busy)
        {
            sleep_us(11);  // PHY write takes 10.24 us
            stat.value = read_mac_mii(mii::stat::Address, mii::stat::RegBank);
        }
    }

    void read_buffer(uint8_t *data, uint8_t len)
    {
        spi_driver_.set_cs(0);

        uint8_t op = read_buffer_memory;
        spi_driver_.write_data(&op);
        spi_driver_.read_data(data, len);

        spi_driver_.set_cs(1);
    }

    void write_buffer(uint8_t *data, uint8_t len = 1)
    {
        spi_driver_.set_cs(0);

        uint8_t op = write_buffer_memory;
        spi_driver_.write_data(&op);
        spi_driver_.write_data(data, len);

        spi_driver_.set_cs(1);
    }

public:
    Enc28j60(SPIDriver& spi_driver)
        : spi_driver_(spi_driver)
    {}

    void reset()
    {
        spi_driver_.set_cs(0);

        uint8_t op = system_reset;
        spi_driver_.write_data(&op);

        spi_driver_.set_cs(1);

        sleep_ms(50);  // errata 1ms
    }

    void init_buffers()
    {
        next_packet_ptr_ = receive_buffer_start;

        write_reg(eth::rxstl::Address, eth::rxstl::RegBank, receive_buffer_start & 0xFF);
        write_reg(eth::rxsth::Address, eth::rxsth::RegBank, receive_buffer_start >> 8);

        write_reg(eth::rxndl::Address, eth::rxndl::RegBank, receive_buffer_end & 0xFF);
        write_reg(eth::rxndh::Address, eth::rxndh::RegBank, receive_buffer_end >> 8);

        write_reg(eth::rxrdptl::Address, eth::rxrdptl::RegBank, receive_buffer_start & 0xFF);
        write_reg(eth::rxrdpth::Address, eth::rxrdpth::RegBank, receive_buffer_start >> 8);

        // TODO: tx buff setup required?
        write_reg(eth::txstl::Address, eth::txstl::RegBank, transmit_buffer_start & 0xFF);
        write_reg(eth::txsth::Address, eth::txsth::RegBank, transmit_buffer_start >> 8);

        write_reg(eth::txndl::Address, eth::txndl::RegBank, transmit_buffer_end & 0xFF);
        write_reg(eth::txndh::Address, eth::txndh::RegBank, transmit_buffer_end >> 8);
    }

    void init_receive_filters()
    {
        eth::rxfcon rxfcon;
        rxfcon.ucen = 1;
        rxfcon.crcen = 1;
        rxfcon.pmen = 1;
        write_reg(rxfcon.Address, rxfcon.RegBank, rxfcon.value);

        write_reg(eth::pmm0::Address, eth::pmm0::RegBank, 0x3f);
        write_reg(eth::pmm1::Address, eth::pmm1::RegBank, 0x30);
        write_reg(eth::pmcsl::Address, eth::pmcsl::RegBank, 0xf9);
        write_reg(eth::pmcsh::Address, eth::pmcsh::RegBank, 0xf7);
    }

    void init_mac(uint8_t* macaddr)
    {
        mac::con1 con1;
        con1.rxen = 1;
        con1.txpaus = 1;
        con1.rxpaus = 1;
        write_reg(con1.Address, con1.RegBank, con1.value);

        mac::con3 con3;
        con3.padcfg = 1;
        con3.txcrcen = 1;
        // con3.fuldpx = 1;
        con3.frmlnen = 1;
        write_reg(con3.Address, con3.RegBank, con3.value, bit_field_set);

        write_reg(mac::mxfll::Address, mac::mxfll::RegBank, max_frame_len & 0xFF);
        write_reg(mac::mxflh::Address, mac::mxflh::RegBank, max_frame_len >> 8);

        write_reg(mac::bbipg::Address, mac::bbipg::RegBank, 0x12);

        write_reg(mac::ipgl::Address, mac::ipgl::RegBank, 0x12);
        write_reg(mac::ipgh::Address, mac::ipgh::RegBank, 0x0C);

        write_reg(mac::adr1::Address, mac::adr1::RegBank, macaddr[0]);
        write_reg(mac::adr2::Address, mac::adr2::RegBank, macaddr[1]);
        write_reg(mac::adr3::Address, mac::adr3::RegBank, macaddr[2]);
        write_reg(mac::adr4::Address, mac::adr4::RegBank, macaddr[3]);
        write_reg(mac::adr5::Address, mac::adr5::RegBank, macaddr[4]);
        write_reg(mac::adr6::Address, mac::adr6::RegBank, macaddr[5]);
    }

    void init_phy()
    {
        // phy::con1 con1;
        // con1.pdpxmd = 1;
        // write_phy(con1.Address, con1.RegBank, con1.value);

        // half duplex
        phy::con2 con2;
        con2.hdldis = 1;
        write_phy(con2.Address, con2.value);
    }

    void enable_receive()
    {
        common::con1 con1x;
        con1x.rxen = 1;
        write_reg(con1x.Address, con1x.RegBank, con1x.value, bit_field_set);
    }

    void init(uint8_t *macaddr)
    {
        spi_driver_.set_cs(1);
        reset();
        init_buffers();
        init_receive_filters();
        init_mac(macaddr);
        init_phy();

        common::ie ie;
        ie.intie = 1;
        ie.pktie = 1;
        write_reg(ie.Address, ie.RegBank, ie.value, bit_field_set);

        enable_receive();
    }

    uint16_t packet_receive(uint16_t maxlen, uint8_t *packet)
    {
        if (read_eth(eth::pktcnt::Address, eth::pktcnt::RegBank) == 0)
        {
            return 0;
        }

        write_reg(eth::rdptl::Address, eth::rdptl::RegBank, next_packet_ptr_ & 0xFF);
        write_reg(eth::rdpth::Address, eth::rdpth::RegBank, next_packet_ptr_ >> 8);

        uint8_t data[2] = {0};
        // autoinc enabled by default
        read_buffer(&data[0], 1);
        read_buffer(&data[1], 1);
        next_packet_ptr_ = data[0] | (data[1] << 8);
        uint16_t len;
        read_buffer(&data[0], 1);
        read_buffer(&data[1], 1);
        len = data[0] | (data[1] << 8);
        uint16_t rxstat;
        read_buffer(&data[0], 1);
        read_buffer(&data[1], 1);
        rxstat = data[0] | (data[1] << 8);

        len -= 4;  // CRC
        // len = std::min<uint16_t>(len, maxlen - 1);
        if (len > maxlen - 1)
        {
            len = maxlen - 1;
        }

        if ((rxstat & 0x80) == 0)
        {
            // invalid
            len = 0;
        }
        else
        {
            // copy the packet from the receive buffer
            read_buffer(packet, len);
        }

        write_reg(eth::rxrdptl::Address, eth::rxrdptl::RegBank, next_packet_ptr_ & 0xFF);
        write_reg(eth::rxrdpth::Address, eth::rxrdpth::RegBank, next_packet_ptr_ >> 8);

        common::con2 con2;
        con2.pktdec = 1;
        write_reg(con2.Address, con2.RegBank, con2.value, bit_field_set);

        return len;
    }

    void packet_send(uint16_t len, uint8_t *packet)
    {
        // Set the write pointer to start of transmit buffer area
        write_reg(eth::wrptl::Address, eth::wrptl::RegBank, transmit_buffer_start & 0xFF);
        write_reg(eth::wrpth::Address, eth::wrpth::RegBank, transmit_buffer_start >> 8);

        // Set the TXND pointer to correspond to the packet size given
        write_reg(eth::txndl::Address, eth::txndl::RegBank, (transmit_buffer_start + len) & 0xFF);
        write_reg(eth::txndh::Address, eth::txndh::RegBank, (transmit_buffer_start + len) >> 8);
        // write per-packet control byte (0x00 means use macon3 settings)
        uint8_t data = 0;
        write_buffer(&data);
        // copy the packet into the transmit buffer
        write_buffer(packet, len);

        // send the contents of the transmit buffer onto the network
        common::con1 con1;
        con1.txrts = 1;
        write_reg(con1.Address, con1.RegBank, con1.value, bit_field_set);
        // Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
        // http://ww1.microchip.com/downloads/en/DeviceDoc/80349c.pdf

        common::ir ir;
        ir.value = read_eth(ir.Address, ir.RegBank);
        if (ir.txerif)
        {
            write_reg(con1.Address, con1.RegBank, con1.value, bit_field_clear);
        }

        // status vector: TXND + 1;
        // uint16_t status = ((read(ETXNDH) << 8) | read(ETXNDL)) + 1;
    }

    uint8_t revision()
    {
        return read_eth(eth::revid::Address, eth::revid::RegBank);
    }
};

#endif // ENC28J60_H

