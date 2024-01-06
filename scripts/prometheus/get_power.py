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

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.sock.close()

    def receive(self, size = 20):
        return self.sock.recv(size)

    def send(self, data, ip = UDP_DEVICE_IP, port = UDP_HOST_PORT):
        self.sock.sendto(data, (ip, port))


def receiver():
    with UDPCommunicator() as comm:
        comm.send(COMM_MAGIC + b'R', port = UDP_DEVICE_PORT)
        data = comm.receive()

    magic, payload = data[0:4], data[4:]

    if magic == COMM_MAGIC:
        bus, shunt, current, power = struct.unpack('ffff', payload)

        print(f'power_bus_voltage_v {bus}')
        print(f'power_shunt_voltage_mv {shunt}')
        print(f'power_current_ma {current}')
        print(f'power_power_mw {power}')


if __name__ == '__main__':
    receiver()
