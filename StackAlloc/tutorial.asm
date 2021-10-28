	@ selects the target architechture as ARM version 6
	.arch armv6
	@ sets the values of different attributes for the "embedded-application binary interface" like file formats, data types, register usages, etc.
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	@ the file name
	.file	"stack_tutorial.c"
	@ tells the assembler to switch to the text segment where the code goes
	.text
	@ align the data immediately after the directive on a memory address that is a multiple of the value that follows it
	.align	2
	@ marks foo as global in the ELF file, which has data for all symbols
	.global	foo
	.arch armv6
	@ signals the use of a unified ARM assembly syntax
	.syntax unified
	@ switches into the correct mode to correctly execute the subsequent code
	.arm
	@ selects the floating point unit as the vfp hardware acclerator
	.fpu vfp
	@ gives the size of the type of its arguments
	.type	foo, %function
foo:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	@ stores a register value into memory
	str	fp, [sp, #-4]!
	@ adds together 2 operands from the last 2 arguments into the first argument
	add	fp, sp, #0
	@ subtraction in the same way as add
	sub	sp, sp, #20
	str	r0, [fp, #-16]
	@ copies the data from the second argument into the first
	mov	r3, #97
	@ calculates an address from a base and offset register value and stores a byte from a register to memory
	strb	r3, [fp, #-5]
	@ does nothing
	nop
	add	sp, fp, #0
	@ sp needed
	@ loads an address into a register
	ldr	fp, [sp], #4
	@ branch and exchange the instructin set
	bx	lr
	.size	foo, .-foo
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu vfp
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	mov	r3, #5
	str	r3, [fp, #-8]
	mov	r3, #6
	str	r3, [fp, #-12]
	ldr	r0, [fp, #-12]
	@ copies the address of the next instruction into the given register and then branches to the label
	bl	foo
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	@ removes the 4-byte data element from the top of the stack into the specified operand
	pop	{fp, pc}
	.size	main, .-main
	@ creates a comment/tag in the object file
	.ident	"GCC: (Raspbian 8.3.0-6+rpi1) 8.3.0"
	@ tags bits of code as different sections
	.section	.note.GNU-stack,"",%progbits
