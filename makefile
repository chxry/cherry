HDD = build/cherry.hdd
KERNEL = build/kernel.elf
VERSION = 0.1.1

SRC = $(shell find kernel/ lib/ -type f -name '*.[cs]') vendor/tinyalloc/tinyalloc.c
OBJS = $(SRC:=.o)
APPS = $(shell find apps/ -type f -name '*.c')
APPOBJS = $(APPS:.c=)
LIB = $(shell find lib/ -type f -name '*.c')
LIBOBJS = $(LIB:=.o)

CFLAGS = -c -ffreestanding -fno-stack-protector -fno-stack-check -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -D__cherry_version__=\"$(VERSION)\"
CFLAGS += -Wall -Wextra -Wno-interrupt-service-routine -Wno-int-conversion -Wno-sign-compare -Wno-unused-parameter
CFLAGS += -Ikernel -Ilib -Ivendor
LDFLAGS = -Tmeta/linker.ld -nostdlib
ASFLAGS = -felf64
MAKEFLAGS += --silent
ECHO = echo -e "\033[1m$(1) \033[0m$(2)"
.SUFFIXES:

run: $(HDD)
	$(call ECHO,"running",$(HDD))
#	qemu-system-x86_64 --enable-kvm -cpu host -serial stdio -m 4G -drive file=$(HDD),format=raw
	qemu-system-x86_64 -serial stdio -m 4G -drive file=$(HDD),format=raw

$(HDD): $(KERNEL) $(APPOBJS)
	$(call ECHO,"creating",$(HDD))
	make -C vendor/limine limine-deploy
	make -C vendor/echfs echfs-utils
	rm -f $(HDD)
	head -c 16M /dev/zero > $(HDD)
	parted -s $(HDD) mklabel gpt
	parted -s $(HDD) mkpart primary 2048s 100%
	./vendor/echfs/echfs-utils -g -p0 $(HDD) format 512 > /dev/null
	./vendor/echfs/echfs-utils -g -p0 $(HDD) import $(KERNEL) boot/kernel.elf
	./vendor/echfs/echfs-utils -g -p0 $(HDD) import meta/limine.cfg boot/limine.cfg
	./vendor/echfs/echfs-utils -g -p0 $(HDD) import vendor/limine/limine.sys boot/limine.sys
	find root/ -type f | xargs -I % ./vendor/echfs/echfs-utils -g -p0 $(HDD) import % %
	find apps/ -type f  ! -name "*.*" | xargs -I % ./vendor/echfs/echfs-utils -g -p0 $(HDD) import % %
	./vendor/limine/limine-deploy $(HDD) 2> /dev/null

$(KERNEL): $(OBJS)
	$(call ECHO,"linking",$(KERNEL))
	mkdir -p build
	ld.lld $(OBJS) $(LDFLAGS) -o $@

%.c.o: %.c
	$(call ECHO,"clang",$<)
	clang $(CFLAGS) $< -o $@

%.s.o: %.s
	$(call ECHO,"nasm",$<)
	nasm $(ASFLAGS) $< -o $@

%: %.c.o meta/start.c.o
	$(call ECHO,"linking",$@)
	ld --oformat binary meta/start.c.o $(LIBOBJS) $< -o $@

format:
	$(call ECHO,"clang-format","linting")
	clang-format -i $(shell find kernel/ apps/ -type f -name '*.[ch]')

clean:
	$(call ECHO,"clean","removing objects")
	rm -rf build $(OBJS) $(APPOBJS) $(LIBOBJS)
