#!/usr/bin/make -f

CC         := gcc
INCLUDE    := -I../kernel/include
CFLAGS	   := -m32 -fno-pic -pipe -nostdlib -nostdinc -ffreestanding -fms-extensions -Wno-int-to-pointer-cast

.PHONY: test

define compile
$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $<
endef

base_dir := ../kernel/core/extend/
base_objects := 	stacks.o		\
					task_manager.o	\
					page_manager.o

objects := $(addprefix $(base_dir), $(base_objects))

test: all
	@./test_script.sh ${objects}

all: clean ${objects}

%.o: %.c
	@$(compile)

clean:
	@rm -f ${objects} test
