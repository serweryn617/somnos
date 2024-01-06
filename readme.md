# Somnos

IoT USB power measurement and on/off switch.

---

The device has USB C input and standard USB A output.

It is capable of measuring USB bus voltage, current and power as well as switching the power on and off.

When requested, the measurement result can be sent over ethernet (UDP) to a receiver, for example to a PC.

⚠️ Power consumed by the device itself is also included in this measurement, which may be unwanted in some use cases.

# Building Firmware

Setup build environment:

```bash
./project.py submodules
./project.py setup
```

Build the project:

```bash
./project.py build
```

# Hardware Used

- Raspberry Pi Pico microcontroller,
- INA219 I2C Current/Power Monitor breakout board,
- Enc28J60 Ethernet Controller board,
- MOSFET breakout board,
- Custom breakout PCB,
- 3D printed enclosure and 4 M3x8 screws.