i=0

while [ $i -lt 4 ]; do
  python3 <script path>/get_power.py > power_tmp
  mv power_tmp <prometheus scrape path>/power.prom
  sleep 15
  i=$(( i + 1 ))
done
