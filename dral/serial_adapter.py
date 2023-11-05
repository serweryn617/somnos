import dral
import yaml

from pathlib import Path


class SerialDeviceAdapter(dral.adapter.BaseAdapter):
    def __init__(self, input: Path):
        self._input = input
        print(self._input)

    def convert(self) -> dral.Device:
        with open(self._input) as data:
            data = yaml.load(data, Loader=yaml.FullLoader)
        device = data["device"]

        # Create base peripheral
        if "peripherals" not in device:
            device["peripherals"] = []
        if "registers" in device:
            device["peripherals"].append({"name": "base", "registers": device["registers"]})

        peripherals = []
        for p in device["peripherals"]:
            registers = []
            for r in p["registers"]:
                fields = []
                for f in r["fields"]:
                    f = dral.Field(**f)
                    fields.append(f)

                # TODO: Mark this field optional in D-RAL
                if "reg_bank" not in r:
                    r["reg_bank"] = 0

                r["fields"] = fields
                r = dral.Register(**r)
                registers.append(r)

            p["registers"] = registers
            p = dral.Peripheral(**p)
            peripherals.append(p)

        return dral.Device(device["name"], device["description"], peripherals)


def main():
    dral.override_adapter(SerialDeviceAdapter)
    dral.cli()


if __name__ == "__main__":
    main()
