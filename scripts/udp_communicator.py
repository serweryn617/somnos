import signal
import socket
import struct
import sys
import time


UDP_DEVICE_IP = '192.168.1.111'
UDP_DEVICE_PORT = 4444
UDP_HOST_PORT = 32000  # Any free port on the host
COMM_MAGIC = b'SMNS'


class UDPCommunicator:
    def __init__(self, ip = '', port = UDP_HOST_PORT):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((ip, port))

    def close(self):
        self.sock.close()

    def receive(self, size = 20):
        return self.sock.recv(size)

    def send(self, data, ip = UDP_DEVICE_IP, port = UDP_HOST_PORT):
        self.sock.sendto(data, (ip, port))


def receiver():
    comm = UDPCommunicator()

    def handler(sig, frame):
        comm.close()
        sys.exit(0)

    signal.signal(signal.SIGINT, handler)

    while True:
        comm.send(COMM_MAGIC + b'R', port = UDP_DEVICE_PORT)
        data = comm.receive()

        magic, payload = data[0:4], data[4:]

        if magic == b'SMNS':
            bus, shunt, current, power = struct.unpack('ffff', payload)

            print()
            print(f'Bus: {bus:.3f} V')
            print(f'Shunt: {shunt:.3f} mV')
            print(f'Current: {current:.3f} mA')
            print(f'Power: {power:.3f} mW')

        time.sleep(1)


if __name__ == '__main__':
    receiver()