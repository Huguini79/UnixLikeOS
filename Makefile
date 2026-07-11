OBJECT_FILES = build/main.o build/entry.asm.o build/ioport.asm.o build/console.o build/printk.o build/gdt.o build/pic.o build/traps.o build/sched.o build/panic.o build/signal.o build/keyboard.o build/alarm.o

CC_FLAGS = -c -I./ -m32 -ffreestanding -nostdlib -fno-stack-protector -fno-pie
AS_FLAGS = -f elf32
LD_FLAGS = -m elf_i386 -T linker.ld -o iso/boot/kernel $(OBJECT_FILES)

TARGET = UnixLikeOS.iso

CC = gcc $(CC_FLAGS)
AS = nasm $(AS_FLAGS)
LD = ld $(LD_FLAGS)
GRUB-MKRESCUE = grub-mkrescue -o $(TARGET) iso/
QEMU = qemu-system-x86_64

all:
	clear
	mkdir -p build

	$(AS) entry.asm -o build/entry.asm.o
	$(AS) arch/x86/ioport.asm -o build/ioport.asm.o

	$(CC) arch/x86/gdt.c -o build/gdt.o
	$(CC) arch/x86/pic.c -o build/pic.o
	$(CC) arch/x86/traps.c -o build/traps.o
	$(CC) arch/x86/keyboard.c -o build/keyboard.o

	$(CC) kernel/console.c -o build/console.o
	$(CC) kernel/printk.c -o build/printk.o
	$(CC) kernel/main.c -o build/main.o
	$(CC) kernel/sched.c -o build/sched.o
	$(CC) kernel/panic.c -o build/panic.o
	$(CC) kernel/signal.c -o build/signal.o
	$(CC) kernel/alarm.c -o build/alarm.o

	$(LD)

	$(GRUB-MKRESCUE)

	$(QEMU) $(TARGET)
