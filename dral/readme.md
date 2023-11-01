# Generating D-RAL

Setup: 

```bash
git clone git@github.com:serweryn617/d-ral-serial.git
python3 -m venv venv
source venv/bin/activate
pip install -e d-ral-serial
```

Generate:

```bash
cd dral
python3 serial_adapter.py enc28j60/enc28j60.yaml enc28j60_out -t serial -T enc28j60/template/
python3 serial_adapter.py ina219/ina219.yaml ina219_out -t serial -T ina219/template/
```

Copy output to repository:

```bash
mkdir -p ../regs/enc28j60/
cp -r enc28j60_out/dralOutput/cpp/* ../regs/enc28j60/

mkdir -p ../regs/ina219/
cp -r ina219_out/dralOutput/cpp/* ../regs/ina219/
```