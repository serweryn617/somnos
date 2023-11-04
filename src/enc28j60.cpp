#include "enc28j60_new.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "enc28j60/common.h"
#include "enc28j60/eth.h"
#include "enc28j60/mac.h"
#include "enc28j60/mii.h"

// The RXSTART_INIT should be zero. See Rev. B4 Silicon Errata
// buffer boundaries applied to internal 8K ram
// the entire available packet buffer space is allocated
//
// start with recbuf at 0/
#define RXSTART_INIT 0x0
// receive buffer end
#define RXSTOP_INIT (0x1FFF - 0x0600 - 1)
// start TX buffer at 0x1FFF-0x0600, pace for one full ethernet frame (~1500 bytes)
#define TXSTART_INIT (0x1FFF - 0x0600)
// stp TX buffer at end of mem
#define TXSTOP_INIT 0x1FFF
//
// max frame length which the conroller will accept:
#define MAX_FRAMELEN 1500 // (note: maximum ethernet frame length would be 1518)
//#define MAX_FRAMELEN     600

// PHY registers
#define PHCON1 0x00
#define PHSTAT1 0x01
#define PHHID1 0x02
#define PHHID2 0x03
#define PHCON2 0x10
#define PHSTAT2 0x11
#define PHIE 0x12
#define PHIR 0x13
#define PHLCON 0x14

// ENC28J60 PHY PHCON1 Register Bit Definitions
#define PHCON1_PRST 0x8000
#define PHCON1_PLOOPBK 0x4000
#define PHCON1_PPWRSV 0x0800
#define PHCON1_PDPXMD 0x0100
// ENC28J60 PHY PHSTAT1 Register Bit Definitions
#define PHSTAT1_PFDPX 0x1000
#define PHSTAT1_PHDPX 0x0800
#define PHSTAT1_LLSTAT 0x0004
#define PHSTAT1_JBSTAT 0x0002
// ENC28J60 PHY PHCON2 Register Bit Definitions
#define PHCON2_FRCLINK 0x4000
#define PHCON2_TXDIS 0x2000
#define PHCON2_JABBER 0x0400
#define PHCON2_HDLDIS 0x0100

using namespace dral::enc28j60;

void Enc28j60::cs_enable()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(mCsPin, 0);
    asm volatile("nop \n nop \n nop");
}

void Enc28j60::cs_disable()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(mCsPin, 1);
    asm volatile("nop \n nop \n nop");
}

void Enc28j60::spi_write_single(uint8_t data)
{
    spi_write_blocking(mSpiInst, &data, 1);
}

uint8_t Enc28j60::read_op(uint8_t op, uint8_t address)
{
    cs_enable();

    // issue read command
    spi_write_single(op | address);

    // read data
    uint8_t dst[1];
    spi_read_blocking(mSpiInst, 0, dst, 1);

    // do dummy read if needed (for mac and mii, see datasheet page 29)
    if (address & 0x80)
    {
        spi_read_blocking(mSpiInst, 0, dst, 1);
    }

    cs_disable();
    return (dst[0]);
}

void Enc28j60::write_op(uint8_t op, uint8_t address, uint8_t data)
{
    cs_enable();

    // issue write command
    spi_write_single(op | address);

    // write data
    spi_write_single(data);

    cs_disable();
}

void Enc28j60::read_buffer(uint16_t len, uint8_t *data)
{
    cs_enable();

    // issue read command
    spi_write_single(read_buffer_memory);

    spi_read_blocking(mSpiInst, 0, data, len);

    cs_disable();
}

void Enc28j60::write_buffer(uint16_t len, uint8_t *data)
{
    cs_enable();

    // issue write command
    spi_write_single(write_buffer_memory);

    printf("writing %d bytes to SPI\n", len);

    // write byte by byte
    // for (int i = 0; i < len; i+=4) {
    //     printf("byte %d = %02x %02x %02x %02x\n", i, data[i], data[i+1], data[i+2], data[i+3]);
    // }

    // for (int i = 0; i < len; i++) {
    //     // printf("byte %d = %02x\n", i, data[i]);
    //     if (spi_is_writable(mSpiInst) == 0) {
    //         printf("SPI: NO SPACE AVAILABLE FOR WRITE\n");
    //     }
    //     spi_write_blocking(mSpiInst, &data[i], 1);
    // }
    spi_write_blocking(mSpiInst, data, len);

    cs_disable();
}

void Enc28j60::set_bank(uint8_t _bank)
{
    // set the bank (if needed)
    if (_bank != bank)
    {
        // set the bank
        common::con1 con1;
        write_op(bit_field_clear, con1.Address, con1.bsel.Mask << con1.bsel.Position);
        write_op(bit_field_set, con1.Address, _bank);
        bank = _bank;
    }
}

uint8_t Enc28j60::read(uint8_t address, uint8_t _bank)
{
    // set the bank
    set_bank(_bank);
    // do the read
    return read_op(read_control_register, address);
}

void Enc28j60::write(uint8_t address, uint8_t _bank, uint8_t data)
{
    // set the bank
    set_bank(_bank);
    // do the write
    write_op(write_control_register, address, data);
}

void Enc28j60::phy_write(uint8_t address, uint16_t data)
{
    // set the PHY register address
    write(mii::regadr::Address, mii::regadr::RegBank, address);
    // write the PHY data
    write(mii::wrl::Address, mii::wrl::RegBank, data);
    write(mii::wrh::Address, mii::wrh::RegBank, data >> 8);
    // wait until the PHY write completes
    mii::stat stat;
    stat.value = read(mii::stat::Address, mii::stat::RegBank);
    while(stat.busy)
    {
        sleep_ms(15);
        stat.value = read(mii::stat::Address, mii::stat::RegBank);
    }
}

void Enc28j60::clkout(uint8_t clk)
{
    //setup clkout: 2 is 12.5MHz:
    write(eth::cocon::Address, eth::cocon::RegBank, clk & 0x7);
}

void Enc28j60::init(uint8_t *macaddr)
{
    // initialize I/O
    // ss as output:
    // pinMode(ENC28J60_CONTROL_CS, OUTPUT);
    //CSPASSIVE; // ss=0
    cs_disable();
    //
    // pinMode(SPI_MOSI, OUTPUT);
    // pinMode(SPI_SCK, OUTPUT);
    // pinMode(SPI_MISO, INPUT);

    // digitalwrite(SPI_MOSI, LOW);
    // digitalwrite(SPI_SCK, LOW);

    // initialize SPI interface
    // master mode and Fosc/2 clock:
    //SPCR = (1<<SPE)|(1<<MSTR);
    //SPSR |= (1<<SPI2X);
    // perform system reset
    write_op(system_reset, 0, system_reset);
    sleep_ms(50);
    // check CLKRDY bit to see if reset is complete
    // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
    //while(!(read(ESTAT) & ESTAT_CLKRDY));
    // do bank 0 stuff
    // initialize receive buffer
    // 16-bit transfers, must write low byte first
    // set receive buffer start address
    NextPacketPtr = RXSTART_INIT;
    // Rx start
    write(eth::rxstl::Address, eth::rxstl::RegBank, RXSTART_INIT & 0xFF);
    write(eth::rxsth::Address, eth::rxsth::RegBank, RXSTART_INIT >> 8);
    // set receive pointer address
    write(eth::rxrdptl::Address, eth::rxrdptl::RegBank, RXSTART_INIT & 0xFF);
    write(eth::rxrdpth::Address, eth::rxrdpth::RegBank, RXSTART_INIT >> 8);
    // RX end
    write(eth::rxndl::Address, eth::rxndl::RegBank, RXSTOP_INIT & 0xFF);
    write(eth::rxndh::Address, eth::rxndh::RegBank, RXSTOP_INIT >> 8);
    // TX start
    write(eth::txstl::Address, eth::txstl::RegBank, TXSTART_INIT & 0xFF);
    write(eth::txsth::Address, eth::txsth::RegBank, TXSTART_INIT >> 8);
    // TX end
    write(eth::txndl::Address, eth::txndl::RegBank, TXSTOP_INIT & 0xFF);
    write(eth::txndh::Address, eth::txndh::RegBank, TXSTOP_INIT >> 8);
    // do bank 1 stuff, packet filter:
    // For broadcast packets we allow only ARP packtets
    // All other packets should be unicast only for our mac (MAADR)
    //
    // The pattern to match on is therefore
    // Type     ETH.DST
    // ARP      BROADCAST
    // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
    // in binary these poitions are:11 0000 0011 1111
    // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
    eth::rxfcon rxfcon;
    rxfcon.ucen = 1;
    rxfcon.crcen = 1;
    rxfcon.pmen = 1;
    write(rxfcon.Address, rxfcon.RegBank, rxfcon.value);
    write(eth::pmm0::Address, eth::pmm0::RegBank, 0x3f);
    write(eth::pmm1::Address, eth::pmm1::RegBank, 0x30);
    write(eth::pmcsl::Address, eth::pmcsl::RegBank, 0xf9);
    write(eth::pmcsh::Address, eth::pmcsh::RegBank, 0xf7);
    //
    //
    // do bank 2 stuff
    // enable MAC receive
    mac::con1 con1;
    con1.rxen = 1;
    con1.txpaus = 1;
    con1.rxpaus = 1;
    write(con1.Address, con1.RegBank, con1.value);
    // bring MAC out of reset
    // TODO: why was this here?: write(MACON2, 0x00);
    // enable automatic padding to 60bytes and CRC operations

    mac::con3 con3;
    con3.padcfg = 1;
    con3.txcrcen = 1;
    con3.frmlnen = 1;
    write_op(bit_field_set, con3.Address, con3.value);
    // set inter-frame gap (non-back-to-back)
    write(mac::ipgl::Address, mac::ipgl::RegBank, 0x12);
    write(mac::ipgh::Address, mac::ipgh::RegBank, 0x0C);
    // set inter-frame gap (back-to-back)
    write(mac::bbipg::Address, mac::bbipg::RegBank, 0x12);
    // Set the maximum packet size which the controller will accept
    // Do not send packets longer than MAX_FRAMELEN:
    write(mac::mxfll::Address, mac::mxfll::RegBank, MAX_FRAMELEN & 0xFF);
    write(mac::mxflh::Address, mac::mxflh::RegBank, MAX_FRAMELEN >> 8);
    // do bank 3 stuff
    // write MAC address
    // NOTE: MAC address in ENC28J60 is byte-backward
    write(mac::adr6::Address, mac::adr6::RegBank, macaddr[0]);
    write(mac::adr5::Address, mac::adr5::RegBank, macaddr[1]);
    write(mac::adr4::Address, mac::adr4::RegBank, macaddr[2]);
    write(mac::adr3::Address, mac::adr3::RegBank, macaddr[3]);
    write(mac::adr2::Address, mac::adr2::RegBank, macaddr[4]);
    write(mac::adr1::Address, mac::adr1::RegBank, macaddr[5]);
    // no loopback of transmitted frames
    phy_write(PHCON2, PHCON2_HDLDIS);
    
    // switch to bank 0
    // TODO: Is this required?:
    set_bank(0);
    
    // enable interrutps
    common::ie ie;
    ie.intie = 1;
    ie.pktie = 1;
    write_op(bit_field_set, ie.Address, ie.value);
    // enable packet reception
    common::con1 con1x;
    con1x.rxen = 1;
    write_op(bit_field_set, con1x.Address, con1x.value);
}

uint8_t Enc28j60::revision(void)
{
    return (read(eth::revid::Address, eth::revid::RegBank));
}

void Enc28j60::packet_send(uint16_t len, uint8_t *packet)
{
    // Set the write pointer to start of transmit buffer area
    write(eth::wrptl::Address, eth::wrptl::RegBank, TXSTART_INIT & 0xFF);
    write(eth::wrpth::Address, eth::wrpth::RegBank, TXSTART_INIT >> 8);

    // Set the TXND pointer to correspond to the packet size given
    write(eth::txndl::Address, eth::txndl::RegBank, (TXSTART_INIT + len) & 0xFF);
    write(eth::txndh::Address, eth::txndh::RegBank, (TXSTART_INIT + len) >> 8);
    // write per-packet control byte (0x00 means use macon3 settings)
    write_op(write_buffer_memory, 0, 0x00);
    // copy the packet into the transmit buffer
    write_buffer(len, packet);

    // send the contents of the transmit buffer onto the network
    common::con1 con1;
    con1.txrts = 1;
    write_op(bit_field_set, con1.Address, con1.value);
    // Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
    // http://ww1.microchip.com/downloads/en/DeviceDoc/80349c.pdf

    common::ir ir;
    ir.value = read(ir.Address, ir.RegBank);
    if (ir.txerif)
    {
        write_op(bit_field_clear, con1.Address, con1.value);
    }

    // status vector: TXND + 1;
    // uint16_t status = ((read(ETXNDH) << 8) | read(ETXNDL)) + 1;
}

// Gets a packet from the network receive buffer, if one is available.
// The packet will by headed by an ethernet header.
//      maxlen  The maximum acceptable length of a retrieved packet.
//      packet  Pointer where packet data should be stored.
// Returns: Packet length in bytes if a packet was retrieved, zero otherwise.
uint16_t Enc28j60::packet_receive(uint16_t maxlen, uint8_t *packet)
{
    uint16_t rxstat;
    uint16_t len;
    // check if a packet has been received and buffered
    //if( !(read(EIR) & EIR_PKTIF) ){
    // The above does not work. See Rev. B4 Silicon Errata point 6.
    if (read(eth::pktcnt::Address, eth::pktcnt::RegBank) == 0)
    {
        // printf("exit: len %d\n", len);
        return (0);
    }

    // Set the read pointer to the start of the received packet
    write(eth::rdptl::Address, eth::rdptl::RegBank, (NextPacketPtr));
    write(eth::rdpth::Address, eth::rdpth::RegBank, (NextPacketPtr) >> 8);
    // read the next packet pointer
    NextPacketPtr = read_op(read_buffer_memory, 0);
    NextPacketPtr |= read_op(read_buffer_memory, 0) << 8;
    // read the packet length (see datasheet page 43)
    len = read_op(read_buffer_memory, 0);
    len |= read_op(read_buffer_memory, 0) << 8;
    len -= 4; //remove the CRC count
    // read the receive status (see datasheet page 43)
    rxstat = read_op(read_buffer_memory, 0);
    rxstat |= read_op(read_buffer_memory, 0) << 8;
    // limit retrieve length
    if (len > maxlen - 1)
    {
        len = maxlen - 1;
    }
    // check CRC and symbol errors (see datasheet page 44, table 7-3):
    // The ERXFCON.CRCEN is set by default. Normally we should not
    // need to check this.
    if ((rxstat & 0x80) == 0)
    {
        // invalid
        len = 0;
    }
    else
    {
        // copy the packet from the receive buffer
        read_buffer(len, packet);
    }
    // Move the RX read pointer to the start of the next received packet
    // This frees the memory we just read out
    write(eth::rxrdptl::Address, eth::rxrdptl::RegBank, (NextPacketPtr));
    write(eth::rxrdpth::Address, eth::rxrdpth::RegBank, (NextPacketPtr) >> 8);
    // decrement the packet counter indicate we are done with this packet
    common::con2 con2;
    con2.pktdec = 1;
    write_op(bit_field_set, con2.Address, con2.value);

    // printf("Packet len %d\n", len);
    return (len);
}
