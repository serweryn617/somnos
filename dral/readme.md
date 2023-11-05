# Generating D-RAL

Setup: 

```bash
cd dral
git submodule update --init --depth 1 d-ral-serial
python3 -m venv venv
source venv/bin/activate
pip install -e d-ral-serial
```

Generate:

```bash
python3 serial_adapter.py enc28j60/enc28j60.yaml enc28j60_out -t serial -T enc28j60/template/
python3 serial_adapter.py ina219/ina219.yaml ina219_out -t serial -T ina219/template/
```

Copy output to repository:

```bash
cp -r enc28j60_out/dralOutput/cpp/* ../regs/
cp -r ina219_out/dralOutput/cpp/* ../regs/
```