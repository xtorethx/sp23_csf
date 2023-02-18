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
	//%di: unsigned byteval
		cmpw $32, %di 
		jl .non_printable //jump if less than 32
		cmpw $126, %di
		jg .non_printable //jump if greater than 126
		movw %di, %ax //return char byteval
		jmp .done
	.non_printable: 
		movw $46, %ax //return '.'
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
		and $0xf, %r11 //and 1111 to mod
		movb (%r10, %r11), %r9b //decode from table; r9b is char representing mod

		movq %rdi, %rax
		shr $4, %rax //shift right to divide
		movb (%r10, %rax), %r8b //r8b is char representing div

		movb %r8b, 0(%rsi) //sbuf[0] = div
		movb %r9b, 1(%rsi) //sbuf[1] = mod
		movb $0, 2(%rsi) //sbuf[2] = '\0'

		movq %rsi, %rax
		ret
		
	hex_format_offset:
		ret

/* vim:ft=gas:
 */
