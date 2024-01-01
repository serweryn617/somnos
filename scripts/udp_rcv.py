import signal
import socket
import struct
import sys


UDP_PORT = 5001


class UDPReceiver():
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(('', UDP_PORT))

    def receive(self):
        data, _ = self.sock.recvfrom(20)
        return data

    def close(self):
        self.sock.close()


def receiver():
    rcv = UDPReceiver()

    def handler(sig, frame):
        rcv.close()
        sys.exit(0)

    # default_handler = signal.getsignal(signal.SIGINT)
    signal.signal(signal.SIGINT, handler)

    while True:
        data = rcv.receive()
        magic = data[0:4]

        if magic == b'SMNS':
            bus, shunt, current, power = struct.unpack('ffff', data[4:])

            print()
            print(f'Bus: {bus:.3f} V')
            print(f'Shunt: {shunt:.3f} mV')
            print(f'Current: {current:.3f} mA')
            print(f'Power: {power:.3f} mW')


if __name__ == '__main__':
    receiver()