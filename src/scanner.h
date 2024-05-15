#ifndef SCANNER_H_
#define SCANNER_H_

#include <vector>
#include <string>
#include <cstdint>
namespace scanner {

    using PortList = std::vector<std::uint16_t>;
    using IPv4List = std::vector<std::uint32_t>;
    struct Target
    {
        std::uint32_t ipaddress;
        const PortList& ports;

        Target(std::uint32_t ipaddress, const PortList& ports) : ipaddress(ipaddress), ports(ports){}
    };

    bool scanSinglePort(std::uint32_t ip, std::uint16_t port);

    void* scanThread(void* target);

} // namespace scanner


#endif // SCANNER_H_