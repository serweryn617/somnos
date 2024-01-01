import signal
import socket
import struct
import sys
import time


UDP_IP = '192.168.1.111'
UDP_PORT = 5001


class UDPReceiver:
    def receive(self, ip = '', port = UDP_PORT):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((ip, port))
        data, _ = self.sock.recvfrom(20)
        self.close()
        return data

    def close(self):
        self.sock.close()


class UDPSender:
    def send(self, data, ip = UDP_IP, port = UDP_PORT):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.sendto(data, (ip, port))
        self.close()

    def close(self):
        self.sock.close()


def receiver():
    rcv = UDPReceiver()
    snd = UDPSender()

    snd.send(b'SMNSR', port = 4444)
    data = rcv.receive()
    magic = data[0:4]

    if magic == b'SMNS':
        bus, shunt, current, power = struct.unpack('ffff', data[4:])

        print(f'power_bus_voltage_v {bus}')
        print(f'power_shunt_voltage_mv {shunt}')
        print(f'power_current_ma {current}')
        print(f'power_power_mw {power}')


if __name__ == '__main__':
    receiver()
