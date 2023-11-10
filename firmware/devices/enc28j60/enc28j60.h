#ifndef ENC28J60_H
#define ENC28J60_H

#include <algorithm>

#include "regs/enc28j60/common.h"
#include "regs/enc28j60/eth.h"
#include "regs/enc28j60/mac.h"
#include "regs/enc28j60/mii.h"
#include "regs/enc28j60/phy.h"

#include "devices/enc28j60/enc28j60_enums.h"
#include "drivers/concepts/spi_driver_concept.h"
#include "utils/utils.h"

namespace devices::enc28j60 {

template<drivers::concepts::spi_driver_concept SPIDriver>
class Enc28j60 {
private:
    SPIDriver& spi_driver_;
    uint8_t bank_ = 0;
    uint16_t next_packet_ptr_;

    static constexpr unsigned int receive_buffer_start = 0x0;  // errata
    static constexpr unsigned int receive_buffer_end = 0x19EF;
    static constexpr unsigned int transmit_buffer_start = 0x19F0;
    static constexpr unsigned int transmit_buffer_end = 0x1FF0;
    static constexpr unsigned int max_frame_len = 1518;

    void set_cs(cs state) { spi_driver_.set_cs(utils::underlying_type(state)); }

    void set_bank(uint8_t requested_bank, uint8_t address)
    {
        bool common_regs = address >= 0x1B;
        bool no_bank_change = bank_ == requested_bank;

        if (common_regs || no_bank_change) {
            return;
        }

        set_cs(cs::enable);
        dral::enc28j60::common::con1 con1;
        uint8_t command = utils::underlying_type(op::bit_field_clear) | con1.Address;
        spi_driver_.write_data(&command);
        uint8_t data = con1.bsel.Mask << con1.bsel.Position;
        spi_driver_.write_data(&data);
        set_cs(cs::disable);

        sleep_us(1);

        set_cs(cs::enable);
        command = utils::underlying_type(op::bit_field_set) | con1.Address;
        spi_driver_.write_data(&command);
        data = (requested_bank & con1.bsel.Mask) << con1.bsel.Position;
        spi_driver_.write_data(&data);
        set_cs(cs::disable);

        bank_ = requested_bank;
    }

    uint8_t read_eth(uint8_t address, uint8_t bank)
    {
        set_bank(bank, address);

        set_cs(cs::enable);

        uint8_t command = utils::underlying_type(op::read_control_register) | address;
        spi_driver_.write_data(&command);

        uint8_t data;
        spi_driver_.read_data(&data);

        set_cs(cs::disable);
        return data;
    }

    uint8_t read_mac_mii(uint8_t address, uint8_t bank)
    {
        set_bank(bank, address);

        set_cs(cs::enable);

        uint8_t command = utils::underlying_type(op::read_control_register) | address;
        spi_driver_.write_data(&command);

        uint8_t data;
        spi_driver_.read_data(&data);  // dummy byte
        spi_driver_.read_data(&data);

        set_cs(cs::disable);
        return data;
    }

    void write_reg(uint8_t address, uint8_t bank, uint8_t data, op operation = op::write_control_register)
    {
        set_bank(bank, address);

        set_cs(cs::enable);

        uint8_t command = utils::underlying_type(operation) | address;
        spi_driver_.write_data(&command);
        spi_driver_.write_data(&data);

        set_cs(cs::disable);
    }

    uint16_t read_phy(uint8_t address, uint16_t data)
    {
        write_reg(dral::enc28j60::mii::regadr::Address, dral::enc28j60::mii::regadr::RegBank, address);

        dral::enc28j60::mii::cmd cmd;
        cmd.miird = 1;
        write_reg(dral::enc28j60::mii::cmd::Address, dral::enc28j60::mii::cmd::RegBank, cmd.value);

        dral::enc28j60::mii::stat stat;
        stat.value = read_mac_mii(dral::enc28j60::mii::stat::Address, dral::enc28j60::mii::stat::RegBank);
        while (stat.busy) {
            sleep_us(11);  // PHY read takes 10.24 us
            stat.value = read_mac_mii(dral::enc28j60::mii::stat::Address, dral::enc28j60::mii::stat::RegBank);
        }

        write_reg(dral::enc28j60::mii::cmd::Address, dral::enc28j60::mii::cmd::RegBank, 0);

        uint16_t value = 0;
        value = read_mac_mii(dral::enc28j60::mii::rdl::Address, dral::enc28j60::mii::rdl::RegBank);
        value |= read_mac_mii(dral::enc28j60::mii::rdh::Address, dral::enc28j60::mii::rdh::RegBank);
        return value;
    }

    void write_phy(uint8_t address, uint16_t data)
    {
        write_reg(dral::enc28j60::mii::regadr::Address, dral::enc28j60::mii::regadr::RegBank, address);

        write_reg(dral::enc28j60::mii::wrl::Address, dral::enc28j60::mii::wrl::RegBank, data & 0xff);
        write_reg(dral::enc28j60::mii::wrh::Address, dral::enc28j60::mii::wrh::RegBank, data >> 8);

        dral::enc28j60::mii::stat stat;
        stat.value = read_mac_mii(dral::enc28j60::mii::stat::Address, dral::enc28j60::mii::stat::RegBank);
        while (stat.busy) {
            sleep_us(11);  // PHY write takes 10.24 us
            stat.value = read_mac_mii(dral::enc28j60::mii::stat::Address, dral::enc28j60::mii::stat::RegBank);
        }
    }

    void read_buffer(uint8_t* data, uint8_t len)
    {
        set_cs(cs::enable);

        uint8_t op = utils::underlying_type(op::read_buffer_memory);
        spi_driver_.write_data(&op);
        spi_driver_.read_data(data, len);

        set_cs(cs::disable);
    }

    void write_buffer(uint8_t* data, uint8_t len = 1)
    {
        set_cs(cs::enable);

        uint8_t op = utils::underlying_type(op::write_buffer_memory);
        spi_driver_.write_data(&op);
        spi_driver_.write_data(data, len);

        set_cs(cs::disable);
    }

public:
    Enc28j60(SPIDriver& spi_driver)
        : spi_driver_(spi_driver)
    {
    }

    void reset()
    {
        set_cs(cs::enable);

        uint8_t op = utils::underlying_type(op::system_reset);
        spi_driver_.write_data(&op);

        set_cs(cs::disable);

        sleep_ms(1);  // errata
    }

    void init_buffers()
    {
        next_packet_ptr_ = receive_buffer_start;

        write_reg(dral::enc28j60::eth::rxstl::Address, dral::enc28j60::eth::rxstl::RegBank, receive_buffer_start & 0xFF);
        write_reg(dral::enc28j60::eth::rxsth::Address, dral::enc28j60::eth::rxsth::RegBank, receive_buffer_start >> 8);

        write_reg(dral::enc28j60::eth::rxndl::Address, dral::enc28j60::eth::rxndl::RegBank, receive_buffer_end & 0xFF);
        write_reg(dral::enc28j60::eth::rxndh::Address, dral::enc28j60::eth::rxndh::RegBank, receive_buffer_end >> 8);

        write_reg(dral::enc28j60::eth::rxrdptl::Address, dral::enc28j60::eth::rxrdptl::RegBank, receive_buffer_start & 0xFF);
        write_reg(dral::enc28j60::eth::rxrdpth::Address, dral::enc28j60::eth::rxrdpth::RegBank, receive_buffer_start >> 8);

        write_reg(dral::enc28j60::eth::txstl::Address, dral::enc28j60::eth::txstl::RegBank, transmit_buffer_start & 0xFF);
        write_reg(dral::enc28j60::eth::txsth::Address, dral::enc28j60::eth::txsth::RegBank, transmit_buffer_start >> 8);
    }

    void init_receive_filters()
    {
        dral::enc28j60::eth::rxfcon rxfcon;
        rxfcon.ucen = 1;
        rxfcon.crcen = 1;
        rxfcon.pmen = 1;
        write_reg(rxfcon.Address, rxfcon.RegBank, rxfcon.value);

        write_reg(dral::enc28j60::eth::pmm0::Address, dral::enc28j60::eth::pmm0::RegBank, 0x3f);
        write_reg(dral::enc28j60::eth::pmm1::Address, dral::enc28j60::eth::pmm1::RegBank, 0x30);
        write_reg(dral::enc28j60::eth::pmcsl::Address, dral::enc28j60::eth::pmcsl::RegBank, 0xf9);
        write_reg(dral::enc28j60::eth::pmcsh::Address, dral::enc28j60::eth::pmcsh::RegBank, 0xf7);
    }

    void init_mac(uint8_t* macaddr)
    {
        dral::enc28j60::mac::con1 con1;
        con1.rxen = 1;
        con1.txpaus = 1;
        con1.rxpaus = 1;
        write_reg(con1.Address, con1.RegBank, con1.value);

        dral::enc28j60::mac::con3 con3;
        con3.padcfg = 1;
        con3.txcrcen = 1;
        con3.fuldpx = 1;
        con3.frmlnen = 1;
        write_reg(con3.Address, con3.RegBank, con3.value, op::bit_field_set);

        write_reg(dral::enc28j60::mac::mxfll::Address, dral::enc28j60::mac::mxfll::RegBank, max_frame_len & 0xFF);
        write_reg(dral::enc28j60::mac::mxflh::Address, dral::enc28j60::mac::mxflh::RegBank, max_frame_len >> 8);

        write_reg(dral::enc28j60::mac::bbipg::Address, dral::enc28j60::mac::bbipg::RegBank, 0x15);

        write_reg(dral::enc28j60::mac::ipgl::Address, dral::enc28j60::mac::ipgl::RegBank, 0x12);

        write_reg(dral::enc28j60::mac::adr1::Address, dral::enc28j60::mac::adr1::RegBank, macaddr[0]);
        write_reg(dral::enc28j60::mac::adr2::Address, dral::enc28j60::mac::adr2::RegBank, macaddr[1]);
        write_reg(dral::enc28j60::mac::adr3::Address, dral::enc28j60::mac::adr3::RegBank, macaddr[2]);
        write_reg(dral::enc28j60::mac::adr4::Address, dral::enc28j60::mac::adr4::RegBank, macaddr[3]);
        write_reg(dral::enc28j60::mac::adr5::Address, dral::enc28j60::mac::adr5::RegBank, macaddr[4]);
        write_reg(dral::enc28j60::mac::adr6::Address, dral::enc28j60::mac::adr6::RegBank, macaddr[5]);
    }

    void init_phy()
    {
        dral::enc28j60::phy::con1 con1;
        con1.pdpxmd = 1;
        write_phy(con1.Address, con1.value);
    }

    void enable_receive()
    {
        dral::enc28j60::common::con1 con1x;
        con1x.rxen = 1;
        write_reg(con1x.Address, con1x.RegBank, con1x.value, op::bit_field_set);
    }

    void init(uint8_t* macaddr)
    {
        set_cs(cs::disable);

        reset();
        init_buffers();
        init_receive_filters();
        init_mac(macaddr);
        init_phy();
        enable_receive();
    }

    uint16_t packet_receive(uint16_t maxlen, uint8_t* packet)
    {
        if (read_eth(dral::enc28j60::eth::pktcnt::Address, dral::enc28j60::eth::pktcnt::RegBank) == 0) {
            return 0;
        }

        write_reg(dral::enc28j60::eth::rdptl::Address, dral::enc28j60::eth::rdptl::RegBank, next_packet_ptr_ & 0xFF);
        write_reg(dral::enc28j60::eth::rdpth::Address, dral::enc28j60::eth::rdpth::RegBank, next_packet_ptr_ >> 8);

        uint16_t len;
        uint16_t rxstat;

        // autoinc enabled by default
        read_buffer(reinterpret_cast<uint8_t*>(&next_packet_ptr_), 2);
        read_buffer(reinterpret_cast<uint8_t*>(&len), 2);
        read_buffer(reinterpret_cast<uint8_t*>(&rxstat), 2);

        len -= 4;  // CRC
        len = std::min<uint16_t>(len, maxlen - 1);

        read_buffer(packet, len);

        write_reg(dral::enc28j60::eth::rxrdptl::Address, dral::enc28j60::eth::rxrdptl::RegBank, next_packet_ptr_ & 0xFF);
        write_reg(dral::enc28j60::eth::rxrdpth::Address, dral::enc28j60::eth::rxrdpth::RegBank, next_packet_ptr_ >> 8);

        dral::enc28j60::common::con2 con2;
        con2.pktdec = 1;
        write_reg(con2.Address, con2.RegBank, con2.value, op::bit_field_set);

        return len;
    }

    void packet_send(uint16_t len, uint8_t* packet)
    {
        write_reg(dral::enc28j60::eth::wrptl::Address, dral::enc28j60::eth::wrptl::RegBank, transmit_buffer_start & 0xFF);
        write_reg(dral::enc28j60::eth::wrpth::Address, dral::enc28j60::eth::wrpth::RegBank, transmit_buffer_start >> 8);

        write_reg(dral::enc28j60::eth::txndl::Address, dral::enc28j60::eth::txndl::RegBank, (transmit_buffer_start + len) & 0xFF);
        write_reg(dral::enc28j60::eth::txndh::Address, dral::enc28j60::eth::txndh::RegBank, (transmit_buffer_start + len) >> 8);

        uint8_t data = 0;
        write_buffer(&data);
        write_buffer(packet, len);

        dral::enc28j60::common::con1 con1;
        con1.txrts = 1;
        write_reg(con1.Address, con1.RegBank, con1.value, op::bit_field_set);
    }

    uint8_t revision() { return read_eth(dral::enc28j60::eth::revid::Address, dral::enc28j60::eth::revid::RegBank); }
};

}  // namespace devices::enc28j60

#endif  // ENC28J60_H
