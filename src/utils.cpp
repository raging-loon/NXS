#include "utils.h"
#include <arpa/inet.h>

using std::vector;
using std::string;

// e.g. 22,23,25,50-100,443
scanner::PortList utils::parsePortList(const char* list)
{
    vector<std::uint16_t> out{};

    auto splitList = utils::split(list, ',');

    size_t pos = 0;
    
    for(auto& i : splitList)
    {
        if((pos = i.find('-')) != string::npos)
        {
            // if we dont check up here, it will be passed to stoi
            // which will execute successfully
            if(pos + 1 == i.length())
            {
                printf("Bad port range: %s\n",i.c_str());
                return {};
            }

            int start = std::stoi(i.substr(0, pos));
            int end =   std::stoi(i.substr(pos + 1));

            if(end > 65535 || start >= end) 
            {
                printf("Bad port range: %s\n",i.c_str());
                return {};
            }
            
            while(start <= end)
                out.push_back(start++);
        }
        
        else 
        {
            
            if(stoi(i) > 65535)
            {
                printf("Bad port: %s\n",i.c_str());
                return {};
            }
            out.push_back(stoi(i));
        }
    }

    return out;
}

scanner::IPv4List utils::parseIpv4AddressList(const char* list)
{
    scanner::IPv4List out{};

    vector<string> temp = split(list,',');
    
    size_t pos = 0;

    // used for ip validation
    std::uint32_t ipv = 0;

    for(auto& i : temp)
    {
        //                                 make sure '-' isn't the last character
        if((pos = i.find('-')) != string::npos && pos + 1 != i.length())
        {
            
            std::uint8_t start   = std::stoi(i.substr(i.rfind('.')+1));
            std::uint8_t end     = std::stoi(i.substr(pos+1));

            // check if it is valid
            if(inet_pton(AF_INET, i.substr(0, pos).c_str(), &ipv) != 1 || end < start)
            {
                printf("Bad IP Range: %s\n", i.c_str());
                return {};    
            }

            ipv = ntohl(ipv);
            
            while(start != end+1)
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