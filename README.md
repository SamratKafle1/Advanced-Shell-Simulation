# Advanced Shell Simulation

A lightweight C-based shell simulation for OS coursework demos. The program includes:

- Login/authentication with SHA-256 password verification and default users.
- Role/permission flow (`admin` and `standard`) with `su`/`sudo` behavior.
- Command passthrough to host shell (supports pipelines like `ls | grep txt`).
- Scheduling demo outputs for Round-Robin and Priority modes.
- Paging demo outputs for FIFO and LRU replacement.
- Synchronization demo using producer/consumer with `pthread` mutex + condition variables.

## Repository layout

- `shell_sim.c` — simulator implementation.
- `Makefile` — build targets (`make`, `make clean`).
- `sample_logfile.log` — data for grep/sort pipeline demo.
- `notes.txt` — sample `.txt` entry for pipe demo.
- `admin_secret.txt` — admin-only target file in permission demo.
- `scripts/run_demo.sh` — non-interactive end-to-end demonstration script.
- `docs/DELIVERABLE4_REPORT_APA.md` — APA-style report template.

## Build

```bash
make clean && make
```

## Run interactively

```bash
./shell
```

Default users (created in `users.db` on first run):

- Admin: `admin` / `admin123`
- Standard: `user` / `user123`

## Demonstration commands

After login:

```bash
# piping
ls | grep txt
cat sample_logfile.log | grep error | sort

# scheduling
scheduler rr
scheduler priority

# paging
paging fifo
paging lru

# synchronization
sync_demo

# permissions
su user user123
cat admin_secret.txt
sudo admin123
cat admin_secret.txt
```

## Automated validation

Run the scripted demonstration and capture output:

```bash
bash scripts/run_demo.sh
```

This writes `demo_output.txt` with a complete transcript you can cite in a paper appendix.

## Screenshot checklist for Deliverable 4

Save terminal screenshots with these names:

- `login.png`
- `piping.png`
- `scheduling_rr.png`
- `scheduling_priority.png`
- `paging_fifo.png`
- `paging_lru.png`
- `sync.png`
- `permission_denial.png`
- `permission_success.png`

You can place them in `docs/screenshots/` for submission packaging.
