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

    def handler(sig, frame):
        rcv.close()
        snd.close()
        sys.exit(0)

    signal.signal(signal.SIGINT, handler)

    while True:
        snd.send(b'SMNSR', port = 4444)
        data = rcv.receive()
        magic = data[0:4]

        if magic == b'SMNS':
            bus, shunt, current, power = struct.unpack('ffff', data[4:])

            print()
            print(f'Bus: {bus:.3f} V')
            print(f'Shunt: {shunt:.3f} mV')
            print(f'Current: {current:.3f} mA')
            print(f'Power: {power:.3f} mW')

        time.sleep(1)


if __name__ == '__main__':
    receiver()