import signal
import socket
import sys


UDP_PORT = 5001


class UDPReceiver():
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(("", UDP_PORT))

    def receive(self):
        data, addr = self.sock.recvfrom(1024)
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
        value = data[1] | (data[2] << 8)
        if chr(data[0]) == "B":
            print("Bus:", value / 250, 'V')
        if chr(data[0]) == "S":
            print("Shunt:", value / 100, 'mV')
        if chr(data[0]) == "C":
            print("Current:", value * 0.0305, 'mA')


if __name__ == "__main__":
    receiver()