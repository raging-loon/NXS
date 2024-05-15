#include "scanner.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <cassert>

// 0.5 second timeout
constexpr struct timeval timeout = {.tv_sec = 0, .tv_usec = 500000};

bool scanner::scanSinglePort(std::uint32_t ip, std::uint16_t port)
{
    bool status = false;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        perror("socket creation");
        return status;
    }
    // inet_pton is not need here
    // because it has been precalulated in utils::parseIpv4AddressList
    struct sockaddr_in in { 
        .sin_family = AF_INET, 
        .sin_port = htons(port),
        .sin_addr = ntohl(ip),
    };
    
    // Set timeout
    if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeval)) < 0)
    {
        perror("setsockopt");
        return false;
    }

    if(connect(sockfd, (const sockaddr*)&in, sizeof(sockaddr_in)) == 0)
        status = true; // It's open!

    close(sockfd);  
    return status;

}

void* scanner::scanThread(void* target)
{
    Target* t = reinterpret_cast<Target*>(target);
    
    // buffer for printing IPs
    char printIPBuffer[16];
    // buffer for hold big-endian IP addresses
    uint32_t reverseIP;

    for(const auto& i : t->ports)
    {
        if(scanSinglePort(t->ipaddress, i))
        {
            reverseIP = htonl(t->ipaddress);
            printf("%s:%d open\n",
                    inet_ntop(AF_INET, &reverseIP, printIPBuffer, 16), i);
        }
    }
    return nullptr;
}