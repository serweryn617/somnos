# Example Prometheus Scripts

Scripts that can be used to automatically get data from the device and prepare it for [Prometheus](https://prometheus.io/).

## Usage

Display power data manually:

```bash
python3 get_power.py
```

---

Measure power every 15s using cron:

Edit crontab:

```bash
crontab -e
```

Add the following entry:

```bash
* * * * * <script path>/power.sh
```