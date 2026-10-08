.section .multiboot
.align 8
multiboot_header_start:
    /* Magic number for Multiboot 2 */
    .long 0xe85250d6
    /* Architecture: 0 = 32-bit i386 Protected Mode */
    .long 0
    /* Header length calculation */
    .long multiboot_header_end - multiboot_header_start
    /* Checksum requirement */
    .long -(0xe85250d6 + 0 + (multiboot_header_end - multiboot_header_start))

    /* Tag: Request framebuffer initialization */
    .align 8
    .short 5                 /* Type 5 = Framebuffer request tag */
    .short 0                 /* Flags */
    .long 20                 /* Size of this graphic tag block */
    .long 1024               /* Width */
    .long 768                /* Height */
    .long 32                 /* Depth bits per pixel */

    /* Tag: End marker requirement */
    .align 8
    .short 0
    .short 0
    .long 8
multiboot_header_end:

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB workspace
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	# Set up stack register
	mov $stack_top, %esp

	# Pass EBX multiboot pointer to C kernel argument
	push %ebx
    push %eax
	call kernel_main

	# Fallback freeze trap loop
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
