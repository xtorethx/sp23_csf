/*
 * Assembly language implementation of hexdump functions
 * D. Zheng   H. Qu
 * dzheng12@jhu.edu   hqu6@jhu.edu
 */
	.section .rodata
sHexDigits: .string "0123456789abcdef"

	/* TODO: add read-only data such as strings here */

	.section .text
	.globl hex_to_printable
	.globl hex_format_byte_as_hex
	.globl hex_format_offset


	/* TODO: add functions here */
	hex_to_printable:	
		cmpw $32, %di
		jl .non_printable //jump if less than 32
		cmpw $126, %di
		jg .non_printable //jump if greater than 126
		movw %di, %ax
		jmp .done
	.non_printable: 
		movw $46, %ax 
		jmp .done
	.done: 
		ret
		

	hex_format_byte_as_hex: 
		// unsigned char byteval as %rdi (%edi), char string sbuf as %rsi (%rsi)

		//di 0x48 72
		//rsi empty string

		and $0xFF, %rdi//take out garbage

		movq $sHexDigits, %r10

		movq %rdi, %r11
		and $0xf, %r11 //r11 is mod
		movb (%r10, %r11), %r9b //decode from table; r9b is char representing mod

		movq %rdi, %rax
		shr $4, %rax 
		movb (%r10, %rax), %r8b //r8b is char representing div

		movb %r8b, 0(%rsi)
		movb %r9b, 1(%rsi)
		movb $0, 2(%rsi)

		movq %rsi, %rax
		ret
		
	hex_format_offset:
		ret

/* vim:ft=gas:
 */
