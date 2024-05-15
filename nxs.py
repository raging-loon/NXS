import socket
import argparse
import time
import threading
def parseIpAddress(ip):
    ipList = []
    # range
    if '-' in ip:
        base, end = ip.split('-')
        end = int(end)
        start = int(base.split('.')[-1])
        base = base[:base.rindex('.') + 1]

        while(start <= end):
            ipList.append(f"{base}{start}")
            start += 1
    elif '/' in ip:
        pass
    else:
        return [ip]
    return ipList

def parsePorts(list):

    portList = []

    for i in list.split(','):
        if '-' in i:
            start, end = i.split('-')
            start = int(start)
            end = int (end)

            if start > end or end > 65535 or start < 0:
                raise ValueError

            while(start <= end):

                portList.append(int(start))
                start += 1
        else:
            if int(i) > 65535 or int(i) < 0:
                raise ValueError
            portList.append(int(i))
    return portList


def commenceScanning(ipList, portList):
    for ip in ipList:
        for port in portList:
            sock = socket.socket()
            sock.settimeout(1)
            try:
                sock.connect((ip, port))
                
                print(f"{ip}:{port} open")

            except Exception as e:
                pass
def main():
    parser = argparse.ArgumentParser('nxs', description='Port Scanner')

    parser.add_argument('-i', '--ip')
    parser.add_argument('-p', '--ports')

    args = parser.parse_args()

    ip_addresses = []
    ports        = []

    if args.ip:
        if not args.ports:
            print("Need ports to scan...")
            return
        for i in args.ip.split(','):
            try:
                ip_addresses += parseIpAddress(i)
            except:
                print(f"Invalid IP address/range: {i}")
                return
    if args.ports:
        if not args.ip:
            print("Need IP adresses to scan...")
            return
        try:
            ports = parsePorts(args.ports)
        except :
            print(f"Invalid port list: {args.ports} ")
            return

    portList = ports
    threadList = []
    while portList != []:
        threadList.append(threading.Thread(target=commenceScanning, args=[ip_addresses, portList[:25]]))
        portList = portList[25:]

    for i in threadList:
        i.start()

if __name__ == '__main__':
    main()