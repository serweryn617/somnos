def calibrate(current_max, shunt_resistance):
    current_lsb = current_max / 2**15
    calibration = 0.04096 / (current_lsb * shunt_resistance)

    print(f'Calibration register value for I_max = {current_max} A and R_shunt = {shunt_resistance} Ohm:')
    print(round(calibration))

    current_register_multiplier = 1000 * current_max / 2**15
    power_register_multiplier = current_register_multiplier * 20

    print('Current register multiplier to get value in mA:')
    print(f'{current_register_multiplier:.3}')
    print('Power register multiplier to get value in mW:')
    print(f'{power_register_multiplier:.3}')