CC = gcc
OUT = wireworld
CFLAGS = -lncurses -g -o $(OUT) $(OUT).c

all:
	$(CC) $(CFLAGS)
debug: all
	lldb $(OUT)
clean:
	rm -r $(OUT) $(OUT).dSYM
