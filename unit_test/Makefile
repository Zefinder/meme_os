#!/usr/bin/make -f

MAKEFLAGS  := --no-print-directory
CC         := gcc
INCLUDE    := -I../kernel/include
CFLAGS	   := -pipe -nostdlib -nostdinc -ffreestanding -fms-extensions

.PHONY: test

define compile
$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $<
endef

exam_dir       := ../tp_exam/
exam_obj   :=	task_manager.o
objects    := $(addprefix $(exam_dir), $(exam_obj))

test: all
	@./test_script.sh ${objects}

all: clean ${objects}

%.o: %.c
	@$(compile)

clean:
	rm -f ${objects}