HDD = build/cherry.hdd
KERNEL = build/kernel.elf
VERSION = 0.1.0

SRC = $(shell find kernel/ -type f -name '*.[cs]') lib/tinyalloc/tinyalloc.c
OBJS = $(SRC:=.o)

CFLAGS = -c -ffreestanding -fno-stack-protector -fno-stack-check -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -D__cherry_version__=\"$(VERSION)\"
CFLAGS += -Ikernel -Ilib 
LDFLAGS = -Tmeta/linker.ld -nostdlib
ASFLAGS = -felf64
MAKEFLAGS += --silent
ECHO = echo -e "\033[1m$(1) \033[0m$(2)"

run: $(HDD)
	$(call ECHO,"running",$(HDD))
	qemu-system-x86_64 --enable-kvm -serial stdio -cpu host -m 4G -drive format=raw,file=$(HDD)

$(HDD): $(KERNEL)
	$(call ECHO,"creating",$(HDD))
	make -C lib/limine limine-deploy
	make -C lib/echfs echfs-utils
	rm -f $(HDD)
	head -c 16M /dev/zero > $(HDD)
	parted -s $(HDD) mklabel gpt
	parted -s $(HDD) mkpart primary 2048s 100%
	./lib/echfs/echfs-utils -g -p0 $(HDD) format 512 > /dev/null
	./lib/echfs/echfs-utils -g -p0 $(HDD) import $(KERNEL) boot/kernel.elf
	./lib/echfs/echfs-utils -g -p0 $(HDD) import meta/limine.cfg boot/limine.cfg
	./lib/echfs/echfs-utils -g -p0 $(HDD) import lib/limine/limine.sys boot/limine.sys
	find root -type f | xargs -I % ./lib/echfs/echfs-utils -g -p0 $(HDD) import % %
	./lib/limine/limine-deploy $(HDD) 2> /dev/null

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

format:
	$(call ECHO,"clang-format","linting")
	clang-format -i $(shell find kernel/ -type f -name '*.[ch]')

clean:
	$(call ECHO,"clean","removing objects")
	rm -rf build $(OBJS)
