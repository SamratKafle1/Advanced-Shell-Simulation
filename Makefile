CC=gcc
CFLAGS=-Wall -Wextra -O2
LDFLAGS=-lssl -lcrypto -lpthread

all: shell

shell: shell_sim.c
	$(CC) $(CFLAGS) -o shell shell_sim.c $(LDFLAGS)

clean:
	rm -f shell
