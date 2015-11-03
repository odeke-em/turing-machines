CC:=gcc
CFLAGS:=-Wall -Wextra

div3: div3.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
