KERNEL = cherry.elf

KSRC = $(shell find kernel/ -type f -name '*.[cs]')
KOBJS = $(KSRC:=.o)
KLD = kernel/kernel.ld

CC = riscv64-elf-gcc
LD = riscv64-elf-ld
AS = riscv64-elf-as
CFLAGS = -ffreestanding -fno-stack-protector -mcmodel=medany -O2 -Wall -Wextra

run: $(KERNEL)
	qemu-system-riscv64 -machine virt -m 128M -smp 1 -nographic -kernel $(KERNEL)

$(KERNEL): $(KOBJS) $(KLD)
	$(LD) -T $(KLD) -nostdlib $(KOBJS) -o $@

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.s.o: %.s
	$(AS) $< -o $@

clean:
	rm -rf $(KOBJS) $(KERNEL)
