#include <stdio.h>

#include "lwip/inet.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/netif.h"
#include "lwip/init.h"
#include "lwip/stats.h"
#include "lwip/dhcp.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"

#include "pico/stdlib.h"
#include "pico/binary_info.h"

#include <stdio.h>
#include <string.h>

#include "ina219.h"
#include "enc28j60.h"
#include "pico_i2c_driver.h"
#include "pico_spi_driver.h"

#define SPI_PORT spi1
#define PIN_SCK 10
#define PIN_MOSI 11
#define PIN_MISO 12
#define PIN_CS 13

// based on example from: https://www.nongnu.org/lwip/2_0_x/group__lwip__nosys.html
#define ETHERNET_MTU 1500


uint8_t mac_addr[6] = {0x11, 0xe8, 0xc3, 0xf8, 0xc6, 0x92};

PicoSPIDriver enc_driver(SPI_PORT, PIN_CS, PIN_MISO, PIN_MOSI, PIN_SCK);
Enc28j60 enc28j60(enc_driver);

struct udp_pcb* upcb;

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port){

	//struct pbuf *p;
	uint8_t data[100]={0};
      sprintf((char*)data, "sending udp client message" );
      /* allocate pbuf from pool*/
      p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)data), PBUF_POOL);
      if (p != NULL)
      {
        /* copy data to pbuf */
        pbuf_take(p, (char*)data, strlen((char*)data));

        /* send udp data */
        udp_send(upcb, p);

        /* free pbuf */
        pbuf_free(p);
      }
}

err_t create_udp_socket(){
    err_t err = ERR_OK;
    ip4_addr_t destIPAddr;
    upcb = udp_new();

    if (upcb == NULL){
        return ERR_MEM;
    }
    // Load the static IP of the destination address
    //169.254.157.160:

    // Tx_buf[30] = 192; // destination IP
//    Tx_buf[31] = 168;
//    Tx_buf[32] = 1;
//    Tx_buf[33] = 49;


    IP4_ADDR(&destIPAddr,192,168,1,26);
    upcb->local_port = 5001;
    //upcb->local_port = 4004; // Set our local port to 4004
    // Should bind to the local ip and port
    err = udp_bind(upcb,IP4_ADDR_ANY,5004);
    if (err != ERR_OK){
        return err;
    }
    // Connect to the other port
    err = udp_connect(upcb,&destIPAddr,5001);
    if (err != ERR_OK){
        return err;
    }
    // Set the receive function
    udp_recv(upcb,udp_receive_callback,NULL);
    return err;
}

err_t send_msg_to_dest(char prefix, uint16_t value){
    struct pbuf *p;
    uint8_t data[100]={0};

    data[0] = prefix;
    data[1] = value & 0xff;
    data[2] = value >> 8;

    /* allocate pbuf from pool*/
    //p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)data), PBUF_POOL);
    p = pbuf_alloc(PBUF_TRANSPORT, 3, PBUF_POOL);

    if (p != NULL)
    {
        /* copy data to pbuf */
        pbuf_take(p, (char*)data, strlen((char*)data));

        /* send udp data */
        udp_send(upcb, p);

        /* free pbuf */
        pbuf_free(p);
        return ERR_OK;
    }
    return ERR_MEM;
}






static err_t netif_output(struct netif *netif, struct pbuf *p)
{
    LINK_STATS_INC(link.xmit);

    // lock_interrupts();
    // pbuf_copy_partial(p, mac_send_buffer, p->tot_len, 0);
    /* Start MAC transmit here */

    printf("enc28j60: Sending packet of len %d\n", p->len);
    enc28j60.packet_send(p->len, (uint8_t *)p->payload);
    // pbuf_free(p);

    // error sending
    dral::enc28j60::common::stat stat;
    stat.value = enc28j60.read(stat.Address, stat.RegBank);
    if (stat.txabrt)
    {
        // a seven-byte transmit status vector will be
        // written to the location pointed to by ETXND + 1,
        printf("ERR - transmit aborted\n");
    }

    dral::enc28j60::common::ir ir;
    ir.value = enc28j60.read(ir.Address, ir.RegBank);
    if (ir.txerif)
    {
        printf("ERR - transmit interrupt flag set\n");
    }

    // unlock_interrupts();
    return ERR_OK;
}

static void netif_status_callback(struct netif *netif)
{
    printf("netif status changed %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
}

static err_t netif_initialize(struct netif *netif)
{
    netif->linkoutput = netif_output;
    netif->output = etharp_output;
    // netif->output_ip6 = ethip6_output;
    netif->mtu = ETHERNET_MTU;
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;
    // MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, 100000000);
    SMEMCPY(netif->hwaddr, mac_addr, sizeof(netif->hwaddr));
    netif->hwaddr_len = sizeof(netif->hwaddr);
    return ERR_OK;
}



int main() {
    // Enable UART so we can print status output
    stdio_init_all();

    // Make the I2C pins available to picotool
    // bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    const uint mosfet_pin = 18;
    gpio_init(mosfet_pin);
    gpio_set_dir(mosfet_pin, GPIO_OUT);
    bool sw = false;
    gpio_put(mosfet_pin, 1);

    ip_addr_t addr, mask, static_ip;
    IP4_ADDR(&static_ip, 192, 168, 1, 111);
    IP4_ADDR(&mask, 255, 255, 255, 0);
    IP4_ADDR(&addr, 192, 168, 1, 1);

    enc_driver.init();
    enc28j60.init(mac_addr);

    struct netif netif;
    lwip_init();
    // IP4_ADDR_ANY if using DHCP client
    netif_add(&netif, &static_ip, &mask, &addr, NULL, netif_initialize, netif_input);
    netif.name[0] = 'e';
    netif.name[1] = '0';
    // netif_create_ip6_linklocal_address(&netif, 1);
    // netif.ip6_autoconfig_enabled = 1;
    netif_set_status_callback(&netif, netif_status_callback);
    netif_set_default(&netif);
    netif_set_up(&netif);

    dhcp_inform(&netif);
    // dhcp_start(&netif);

    uint8_t *eth_pkt = (uint8_t*)malloc(ETHERNET_MTU);
    struct pbuf *p = NULL;

    netif_set_link_up(&netif);
    create_udp_socket();

    PicoI2CDriver ina219_driver(i2c0, 16, 17, 0x40);
    ina219_driver.init();
    INA219<PicoI2CDriver> ina219(ina219_driver);
    ina219.calibrate();

    while (true) {
        // sw = !sw;
        // sleep_ms(100);
        // gpio_put(mosfet_pin, sw);
        // sleep_ms(100);

        // float sh = ina219.shunt_voltage();
        // printf("Shunt voltage: %.2f mV\n", sh);

        // float bus = ina219.bus_voltage();
        // printf("Bus voltage: %.2f V\n", bus);

        uint16_t packet_len = enc28j60.packet_receive(ETHERNET_MTU, (uint8_t *)eth_pkt);
        if (packet_len)
        {
            printf("enc: Received packet of length = %d\n", packet_len);
            p = pbuf_alloc(PBUF_RAW, packet_len, PBUF_POOL);
            pbuf_take(p, eth_pkt, packet_len);
            free(eth_pkt);
            eth_pkt = (uint8_t*)malloc(ETHERNET_MTU);
        }
        else
        {
            // printf("enc: no packet received\n");
        }

        if (packet_len && p != NULL)
        {
            LINK_STATS_INC(link.recv);

            if (netif.input(p, &netif) != ERR_OK)
            {
                pbuf_free(p);
            }
        }

        send_msg_to_dest('B', ina219.bus_voltage_raw());
        send_msg_to_dest('S', ina219.shunt_voltage_raw());
        send_msg_to_dest('C', ina219.current_raw());

        /* Cyclic lwIP timers check */
        sys_check_timeouts();

        sleep_ms(1000);
    }

    return 0;
}
