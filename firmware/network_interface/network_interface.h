#ifndef NETIF_H
#define NETIF_H

// lwIP documentation:
// https://www.nongnu.org/lwip/2_0_x/group__lwip__nosys.html

// references:
// https://github.com/Juddling/pi-pico-enc28j60
// https://github.com/guyg3333/LWIP_udp_example_nucleo_144/

#include "lwip/dhcp.h"
#include "lwip/inet.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/timeouts.h"
#include "lwip/udp.h"
#include "netif/etharp.h"

#include "concepts/network_adapter_concept.h"
#include "somnos/inc/hw.h"  // TODO: remove

namespace network {

template<concepts::network::network_adapter_concept NetworkAdapter>
class Interface {
private:
    NetworkAdapter& adapetr_;
    struct udp_pcb* upcb;  // protocol control block
    struct netif netif;
    uint8_t* eth_pkt;
    struct pbuf* p = NULL;
    ip_addr_t static_ip, mask, gateway;

    err_t create_udp_socket()
    {
        err_t err;

        upcb = udp_new();
        if (upcb == NULL) {
            return ERR_MEM;
        }

        err = udp_bind(upcb, IP4_ADDR_ANY, 4444);
        if (err != ERR_OK) {
            printf("udp_bind %i\n", err);
            return err;
        }

        // Connected pcb can only receive from the connected remote
        // ip4_addr_t destIPAddr;
        // IP4_ADDR(&destIPAddr, 192, 168, 1, 26);
        // err = udp_connect(upcb, &destIPAddr, 5001);
        // if (err != ERR_OK) {
        //     printf("udp_connect %i\n", err);
        //     return err;
        // }

        udp_recv(upcb, udp_receive_callback, NULL);
        return ERR_OK;
    }

    static void udp_receive_callback(void* arg, struct udp_pcb* upcb, struct pbuf* p, const ip_addr_t* address, u16_t port)
    {
        // auto* instance = static_cast<Interface<NetworkAdapter>*>(arg);
        printf("udp_receive_callback: p->len: %d\n", p->len);
        printf("udp_receive_callback: p->payload: ");
        for (size_t i = 0; i < p->len; i++)
        {
            printf("%c", ((char*)p->payload)[i]);
        }
        printf("\n");

        pbuf_free(p);
    }

    static err_t netif_output(struct netif* netif, struct pbuf* p)
    {
        LINK_STATS_INC(link.xmit);

        auto* instance = static_cast<Interface<NetworkAdapter>*>(netif->state);
        instance->adapetr_.packet_send(p->len, (uint8_t*)p->payload);  // TODO: Use tot_len?

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
    Interface(NetworkAdapter& adapetr)
        : adapetr_(adapetr)
    {}

    void init()
    {
        IP4_ADDR(&static_ip, 192, 168, 1, 111);
        IP4_ADDR(&mask, 255, 255, 255, 0);
        IP4_ADDR(&gateway, 192, 168, 1, 1);

        lwip_init();

        netif_add(&netif, &static_ip, &mask, &gateway, this, netif_initialize, netif_input);
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
        uint16_t packet_len = adapetr_.packet_receive(hw::netif::ethernet_mtu, (uint8_t*)eth_pkt);

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

        p = pbuf_alloc(PBUF_TRANSPORT, 3, PBUF_POOL);  // strlen((char*)data)

        if (p == NULL) {
            return ERR_MEM;
        }

        pbuf_take(p, (char*)data, strlen((char*)data));

        // udp_send(upcb, p);  // use udp_sendto for non connected pcb
        ip4_addr_t destIPAddr;
        IP4_ADDR(&destIPAddr, 192, 168, 1, 26);
        udp_sendto(upcb, p, &destIPAddr, 5001);

        pbuf_free(p);
        return ERR_OK;
    }

    void check_timers()
    {
        sys_check_timeouts();
    }
};

}  // namespace network

#endif  // NETIF_H