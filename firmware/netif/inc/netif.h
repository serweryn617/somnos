#ifndef NETIF_H
#define NETIF_H



namespace network {

class interface
{
public:
    interface() {}

    void init_();
    void receive_();
    void send_(char prefix, uint16_t value);
    void check_timers_();
};

}  // namespace netif

#endif  // NETIF_H