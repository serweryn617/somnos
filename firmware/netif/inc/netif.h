#ifndef NETIF_H
#define NETIF_H

#include "lwip/dhcp.h"
#include "lwip/inet.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/timeouts.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "netif/inc/netif_concept.h"

#include "somnos/inc/hw.h"

// lwIP documentation: https://www.nongnu.org/lwip/2_0_x/group__lwip__nosys.html

// inspiration:
// https://github.com/Juddling/pi-pico-enc28j60
// https://github.com/guyg3333/LWIP_udp_example_nucleo_144/

namespace network {

template<network::concepts::netif_concept NetworkInterface>
class interface {
private:
    NetworkInterface& netif_;
    struct udp_pcb* upcb;
    struct netif netif;
    uint8_t* eth_pkt;
    struct pbuf* p = NULL;
    ip_addr_t static_ip, mask, address;

    static void udp_receive_callback(void* arg, struct udp_pcb* upcb, struct pbuf* p, const ip_addr_t* address, u16_t port)
    {
        // struct pbuf *p;
        uint8_t data[100] = { 0 };
        sprintf((char*)data, "sending udp client message");
        /* allocate pbuf from pool*/
        p = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)data), PBUF_POOL);
        if (p != NULL) {
            /* copy data to pbuf */
            pbuf_take(p, (char*)data, strlen((char*)data));

            /* send udp data */
            udp_send(upcb, p);

            /* free pbuf */
            pbuf_free(p);
        }
    }

    err_t create_udp_socket()
    {
        err_t err = ERR_OK;
        ip4_addr_t destIPAddr;
        upcb = udp_new();

        if (upcb == NULL) {
            return ERR_MEM;
        }
        // Load the static IP of the destination address
        // 169.254.157.160:

        // Tx_buf[30] = 192; // destination IP
        //    Tx_buf[31] = 168;
        //    Tx_buf[32] = 1;
        //    Tx_buf[33] = 49;

        IP4_ADDR(&destIPAddr, 192, 168, 1, 26);
        upcb->local_port = 5001;
        // upcb->local_port = 4004; // Set our local port to 4004
        //  Should bind to the local ip and port
        err = udp_bind(upcb, IP4_ADDR_ANY, 5004);
        if (err != ERR_OK) {
            return err;
        }
        // Connect to the other port
        err = udp_connect(upcb, &destIPAddr, 5001);
        if (err != ERR_OK) {
            return err;
        }
        // Set the receive function
        udp_recv(upcb, udp_receive_callback, NULL);
        return err;
    }

    static err_t netif_output(struct netif* netif, struct pbuf* p)
    {
        LINK_STATS_INC(link.xmit);

        auto* instance = static_cast<interface<NetworkInterface>*>(netif->state);
        instance->netif_.packet_send(p->len, (uint8_t*)p->payload);  // TODO: Use tot_len?

        return ERR_OK;
    }

    static void netif_status_callback(struct netif* netif)
    {}

    static err_t netif_initialize(struct netif* netif)
    {
        // TODO: do something with this
        uint8_t mac_addr[6] = { 0x11, 0xe8, 0xc3, 0xf8, 0xc6, 0x92 };

        netif->linkoutput = netif_output;
        netif->output     = etharp_output;
        // netif->output_ip6 = ethip6_output;
        netif->mtu        = hw::netif::ethernet_mtu;
        netif->flags      = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;
        // MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, 100000000);
        SMEMCPY(netif->hwaddr, mac_addr, sizeof(netif->hwaddr));
        netif->hwaddr_len = sizeof(netif->hwaddr);
        return ERR_OK;
    }

public:
    interface(NetworkInterface& network_interface)
        : netif_(network_interface)
    {}

    void init()
    {
        IP4_ADDR(&static_ip, 192, 168, 1, 111);
        IP4_ADDR(&mask, 255, 255, 255, 0);
        IP4_ADDR(&address, 192, 168, 1, 1);

        lwip_init();

        netif_add(&netif, &static_ip, &mask, &address, this, netif_initialize, netif_input);
        netif.name[0] = 'e';
        netif.name[1] = '0';
        // netif_create_ip6_linklocal_address(&netif, 1);
        // netif.ip6_autoconfig_enabled = 1;
        netif_set_status_callback(&netif, netif_status_callback);
        netif_set_default(&netif);
        netif_set_up(&netif);

        dhcp_inform(&netif);

        eth_pkt = (uint8_t*)malloc(hw::netif::ethernet_mtu);

        // if(link_state_changed()) {
        //     if(link_is_up()) {
        //         netif_set_link_up(&netif);
        //     } else {
        //         netif_set_link_down(&netif);
        //     }
        // }
        netif_set_link_up(&netif);

        create_udp_socket();
    }

    void receive()
    {
        uint16_t packet_len = netif_.packet_receive(hw::netif::ethernet_mtu, (uint8_t*)eth_pkt);

        if (!packet_len) {
            return;
        }

        p = pbuf_alloc(PBUF_RAW, packet_len, PBUF_POOL);

        if (p == NULL) {
            return;
        }

        pbuf_take(p, eth_pkt, packet_len);
        // free(eth_pkt);
        // eth_pkt = (uint8_t*)malloc(hw::netif::ethernet_mtu);

        LINK_STATS_INC(link.recv);

        if (netif.input(p, &netif) != ERR_OK) {
            pbuf_free(p);
        }
    }

    err_t send(char prefix, uint16_t value)
    {
        struct pbuf* p;
        uint8_t data[100] = { 0 };

        data[0] = prefix;
        data[1] = value & 0xff;
        data[2] = value >> 8;

        /* allocate pbuf from pool*/
        // p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)data), PBUF_POOL);
        p = pbuf_alloc(PBUF_TRANSPORT, 3, PBUF_POOL);

        if (p != NULL) {
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

    void check_timers()
    {
        sys_check_timeouts();
    }
};

}  // namespace netif

#endif  // NETIF_H