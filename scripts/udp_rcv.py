import socket

UDP_IP = "192.168.1.26"
UDP_PORT = 5001

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024)
    value = data[1] | (data[2] << 8)
    # print("Value:", data[0])
    if chr(data[0]) == "B":
        print("Bus:", value / 250, 'V')
    if chr(data[0]) == "S":
        print("Shunt:", value / 100, 'mV')
    if chr(data[0]) == "C":
        print("Current:", value * 0.0305, 'mA')