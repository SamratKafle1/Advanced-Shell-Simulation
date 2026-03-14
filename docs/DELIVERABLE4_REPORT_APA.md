# Deliverable 4 Report (APA 7 Template)

## Title Page
- Project title
- Student name(s)
- Institution
- Course, instructor
- Date

## Abstract (150–250 words)
Briefly summarize goals, implementation approach, and key results.

## Introduction
- Problem statement.
- Objectives of advanced shell simulation.
- Scope: authentication, piping, scheduling, paging, synchronization, permissions.

## Method / System Design
### Environment
- OS, compiler version, libraries (`openssl`, `pthread`).

### Build and execution procedure
- `make clean && make`
- `./shell`

### User setup/authentication
- Default users and hashed password verification in source.

## Results
### 1. Login flow
- Figure 1: `login.png`
- Observed successful authentication.

### 2. Piping
- Figure 2: `piping.png`
- Commands run:
  - `ls | grep txt`
  - `cat sample_logfile.log | grep error | sort`

### 3. Scheduling
- Figure 3: `scheduling_rr.png` (Round-Robin)
- Figure 4: `scheduling_priority.png` (Priority)
- Compare queue/order differences.

### 4. Paging
- Figure 5: `paging_fifo.png`
- Figure 6: `paging_lru.png`
- Compare victim page differences.

### 5. Synchronization
- Figure 7: `sync.png`
- Explain producer-consumer coordination and signaling.

### 6. Permission model
- Figure 8: `permission_denial.png`
- Figure 9: `permission_success.png`
- Explain standard-user denial and `sudo` elevation flow.

## Discussion
- What worked well.
- Limitations (demo-mode scheduler/paging outputs are simulated, not kernel-level).
- Security caveats (educational simulator vs production shell).

## Conclusion
Summarize outcomes and whether deliverable requirements were met.

## References (APA 7)
Add primary sources, e.g.:
- OpenSSL SHA-256 documentation.
- POSIX shell references.
- `pthread` man pages.
- OS textbook/source for RR, Priority, FIFO, LRU.
- Your GitHub repository URL and tagged release.

## Appendix A: Command Transcript
Include `demo_output.txt`.

## Appendix B: Source Code and Screenshots
- Link to GitHub repo and release tag.
- Include screenshot files under `docs/screenshots/`.
