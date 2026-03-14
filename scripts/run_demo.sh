#!/usr/bin/env bash
set -euo pipefail

make clean >/dev/null
make >/dev/null

cat > .demo_input <<'IN'
admin
admin123
ls | grep txt
cat sample_logfile.log | grep error | sort
scheduler rr
scheduler priority
paging fifo
paging lru
sync_demo
su user user123
cat admin_secret.txt
sudo admin123
cat admin_secret.txt
exit
IN

./shell < .demo_input > demo_output.txt
rm -f .demo_input shell users.db

echo "Demo complete. Output saved to demo_output.txt"
