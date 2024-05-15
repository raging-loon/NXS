#include <stdio.h>
#include <getopt.h>

#include "utils.h"
#include <arpa/inet.h>
#include "scanner.h"

int main(int argc, char** argv)
{
    scanner::IPv4List    ipaddrs;
    scanner::PortList  ports;
    int arg = 0;

    while((arg = getopt(argc, argv, "i:p:")) != -1)
    {
        switch(arg)
        {
            case 'i':
                ipaddrs = utils::parseIpv4AddressList(optarg);
                break;
            case 'p':
                ports = utils::parsePortList(optarg);
                break;
        }
    }

    if(ports.size() == 0)
    {
        printf("Need port[s] to scan...\n");
        return 1;
    }

    if(ipaddrs.size() == 0)
    {
        printf("Need IP Address[es] to scan...\n");
        return 1;
    }
 

    std::vector<pthread_t> threads;
    std::vector<scanner::Target> targets;
    
    // Create scan targets
    for(size_t i = 0; i < ipaddrs.size(); i++)
        targets.emplace_back(ipaddrs[i], ports);

    // Create threads
    for(size_t i = 0; i < targets.size(); i++)
    {    
        pthread_t thr;
        pthread_create(&thr, NULL, scanner::scanThread, &targets[i]);
        threads.push_back(thr);
    }
    // Make sure threads run
    for(auto i : threads)
        pthread_join(i, nullptr);
    
    return 0;
}