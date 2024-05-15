#include "utils.h"
#include <arpa/inet.h>

using std::vector;
using std::string;

// e.g. 22,23,25,50-100,443
scanner::PortList utils::parsePortList(const char* list)
{
    vector<std::uint16_t> out{};

    auto splitList = utils::split(list, ',');

    
    for(auto& i : splitList)
    {
        size_t pos = 0;
        if((pos = i.find('-')) != string::npos)
        {
            // Make sure there is actually something after the hyphen
            if(pos + 1 == i.length())
            {
                printf("Bad port range: %s\n",i.c_str());
                return {};
            }

            int start = std::stoi(i.substr(0, pos));
            int end =   std::stoi(i.substr(pos + 1));
            
            // ensure it is a valid range
            if(end > 65535 || start >= end) 
            {
                printf("Bad port range: %s\n",i.c_str());
                return {};
            }
            // fortunately we don't hvae to deal with endianness-shenanigans here (yet)
            while(start <= end)
                out.push_back(start++);
        }
        
        else 
        {
            pos = stoi(i);
            if(pos > 65535)
            {
                printf("Bad port: %s\n",i.c_str());
                return {};
            }
            out.push_back(pos);
        }
    }

    return out;
}

scanner::IPv4List utils::parseIpv4AddressList(const char* list)
{
    scanner::IPv4List out{};

    vector<string> temp = split(list,',');
    

    // used for ip validation
    std::uint32_t ipv = 0;

    for(auto& i : temp)
    {
        size_t pos = 0;
        //                                 make sure '-' isn't the last character
        if((pos = i.find('-')) != string::npos && pos + 1 != i.length())
        {
            // extract the starting octet and the ending octet
            // E.g. the "1" and "255" from "192.168.1.1-255"
            std::uint8_t start   = std::stoi(i.substr(i.rfind('.')+1));
            std::uint8_t end     = std::stoi(i.substr(pos+1));
            
            // check if it is valid
            if(inet_pton(AF_INET, i.substr(0, pos).c_str(), &ipv) != 1 || end < start)
            {
                printf("Bad IP Range: %s\n", i.c_str());
                return {};    
            }
            // Reverse the endianess back
            ipv = ntohl(ipv);
            
            while(start != end+1)
                // add the difference between start and end
                // e.g. 757145756 + (160-156) = 757145760
                // or   45.33.32.156 + 4 =  45.33.32.160
                // originally this was done with strings so this code looked like
                // out.push_back(base + start++), which produced a minor
                // headache before I figured  out what this issue was.
                out.push_back(ipv + (end - start++) );
        }        
        else
        {
            if(inet_pton(AF_INET, i.c_str(), &ipv) != 1)
            {
                printf("Bad IP Address: %s\n",i.c_str());
                return {};
            }
            out.push_back(ntohl(ipv));
        }

    }

    return out;
}

vector<string> utils::split(const char* src, char delimiter)
{
    vector<string> out{};

    string temp(src);
    size_t pos = 0;
    string tok;

    while((pos = temp.find(delimiter)) != string::npos)
    {
        tok = temp.substr(0, pos);
        out.push_back(tok);
        temp.erase(0,pos+1);
    }
    out.push_back(temp);

    return out;
}