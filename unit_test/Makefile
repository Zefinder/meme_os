#!/usr/bin/make -f

CC         := gcc
INCLUDE    := -I../kernel/include
CFLAGS	   := -pipe -nostdlib -nostdinc -ffreestanding -fms-extensions

.PHONY: test

define compile
$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $<
endef

sources := $(shell find ../tp_exam -name '*.c' -a -not '(' -name 'printutils.c' -o -name 'tp.c' ')')
objects := $(patsubst %.c,%.o,$(sources))

test: all
	@./test_script.sh ${objects}

all: clean ${objects}

%.o: %.c
	@$(compile)

clean:
	@rm -f ${objects} test
