# Tripple Point

A small 32-bit x86 kernel project following the i686-elf and GRUB Multiboot2 tutorial path. The kernel starts in 32-bit protected mode and writes text to the framebuffer requested by its Multiboot2 header.

## Requirements

- `i686-elf-gcc` and `i686-elf-as`
- GRUB utilities: `grub-file` and `grub-mkrescue`
- `xorriso`
- `qemu-system-i386`
- GNU Make

## Build and run

From the repository root:

```sh
make kernel       # Build build/kernel.elf
make check        # Verify the ELF is recognized as Multiboot2
make iso          # Create build/myos.iso
make run          # Boot the ISO in QEMU
make clean        # Remove generated build files
```

`make` builds the ISO. Build products are placed in `build/`; source files stay under `src/`, the linker script under `linker/`, and GRUB configuration under `boot/`.

## Boot and build flow

`src/arch/i386/boot.s` provides the Multiboot2 header and `_start` entry point. GRUB loads `build/kernel.elf`, enters the kernel in 32-bit protected mode, and `_start` establishes a stack before calling `kernel_main` in `src/kernel/kernel.c`. The linker script places the kernel at 2 MiB and keeps the Multiboot2 header near the start of the image. `make iso` stages the ELF and GRUB config, then `grub-mkrescue` packages them into the bootable ISO.

The framebuffer path currently supports 32-bit RGB pixels with the expected RGB channel layout. If GRUB supplies a different or unusable framebuffer, the kernel skips drawing rather than using a truncated address. A serial console is not implemented yet.

## Source map

- `src/arch/i386/boot.s` - Multiboot2 header, stack, and assembly entry
- `src/kernel/kernel.c` - kernel entry, boot-tag parsing, and basic framebuffer terminal
- `linker/i386.ld` - kernel ELF layout and entry symbol
- `boot/grub/grub.cfg` - GRUB menu and kernel/module paths
- `assets/message.txt` - sample module displayed by the kernel

This is an early learning kernel, not a general-purpose OS. Generated artifacts and the previous local build are not source inputs.
