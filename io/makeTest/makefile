target=test
src=$(wildcard *.c)
objects=$(patsubst %.c,%.o,$(src))
CC=gcc
CPPFLAGS=-I ./
$(target): $(objects)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) -c $< $(CPPFLAGS)

.PHONY:clean
clean:
	rm -f $(objects) $(target)
