#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <string>

#include "scanner.h"

namespace utils {

    scanner::PortList parsePortList(const char* list);

    scanner::IPv4List parseIpv4AddressList(const char* list);

    std::vector<std::string> split(const char* src, char delimiter);
}


#endif // UTILS_H_