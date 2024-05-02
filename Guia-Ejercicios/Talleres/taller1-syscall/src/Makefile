CC=gcc

ARCH := $(shell uname -m)

BINS=hai antikill ej1

OBJS=$(BINS:=.o)

all: $(BINS)

# Dependencias
hai.o: hai.c
antikill.o: antikill.c
ej1.o: ej1.c

clean:
	rm -rf $(OBJS) $(BINS)
