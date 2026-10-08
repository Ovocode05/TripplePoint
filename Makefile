CROSS ?= i686-elf-
CC := $(CROSS)gcc
AS := $(CROSS)as
GRUB_FILE ?= grub-file
GRUB_MKRESCUE ?= grub-mkrescue
QEMU ?= qemu-system-i386

BUILD_DIR := build
ISO_ROOT := $(BUILD_DIR)/iso
KERNEL := $(BUILD_DIR)/kernel.elf
ISO := $(BUILD_DIR)/myos.iso
OBJECTS := $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o

CFLAGS := -m32 -march=i686 -std=gnu11 -O2 -Wall -Wextra \
	-ffreestanding -fno-builtin -fno-pie -fno-pic -fno-stack-protector \
	-mno-sse -mno-mmx -msoft-float -Iinclude
LDFLAGS := -m32 -T linker/i386.ld -ffreestanding -nostdlib -no-pie \
	-Wl,-Map=$(BUILD_DIR)/kernel.map

.PHONY: all kernel check iso run debug clean

all: iso

kernel: $(KERNEL)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/boot.o: src/arch/i386/boot.s | $(BUILD_DIR)
	$(AS) --32 -g $< -o $@

$(BUILD_DIR)/kernel.o: src/kernel/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJECTS) linker/i386.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) -lgcc

check: $(KERNEL)
	@command -v $(GRUB_FILE) >/dev/null || { echo "Missing $(GRUB_FILE); install GRUB utilities to check the Multiboot2 image."; exit 1; }
	$(GRUB_FILE) --is-x86-multiboot $(KERNEL)

iso: $(ISO)

$(ISO): $(KERNEL) boot/grub/grub.cfg assets/message.txt
	command -v $(GRUB_MKRESCUE) >/dev/null || { echo "Missing $(GRUB_MKRESCUE); install GRUB utilities to create the ISO."; exit 1; }
	mkdir -p $(ISO_ROOT)/boot/grub
	cp $(KERNEL) $(ISO_ROOT)/boot/kernel.elf
	cp boot/grub/grub.cfg $(ISO_ROOT)/boot/grub/grub.cfg
	cp assets/message.txt $(ISO_ROOT)/boot/message.txt
	$(GRUB_MKRESCUE) -o $(ISO) $(ISO_ROOT)

run: $(ISO)
	$(QEMU) -m 128M -cdrom $(ISO)

debug: $(ISO)
	$(QEMU) -m 128M -cdrom $(ISO) -s -S

clean:
	rm -rf $(ISO_ROOT)
	rm -f $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o $(KERNEL) \
		$(BUILD_DIR)/kernel.map $(ISO)