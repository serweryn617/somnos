#ifndef NETIF_H
#define NETIF_H



namespace network {

class interface
{
public:
    interface() {}

    void init();
    void send(char prefix, uint16_t value);
    void check_timers();
    void receive();
};

}  // namespace netif

#endif  // NETIF_H