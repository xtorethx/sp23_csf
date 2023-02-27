/*
 * Assembly language implementation of hexdump functions
 * D. Zheng   H. Qu
 * dzheng12@jhu.edu   hqu6@jhu.edu
 */
	.section .rodata
sHexDigits: .string "0123456789abcdef"

	/* TODO: add read-only data such as strings here */

	.section .text
	.globl hex_read
	.globl hex_write_string
	.globl hex_format_offset
	.globl hex_format_byte_as_hex
	.globl hex_to_printable
	
	


	/* TODO: add functions here */
	hex_read:
		ret

	hex_write_string: 
		ret
	
	hex_format_offset:
		movl %edi, %r8d //edi = 1, r8d = 1
		movl $0, %r9d //r9d = 0
		movl %edi, %r10d //r10d = 1
	.Ltopforloop:
		cmpl $8, %r9d
		jge .Lendforloop
		andl $0xf, %r8d //r8d = 1
		cmpl $10, %r8d
		jge .Lelsecondition
		addl $48, %r8d //r8d = 49
		jmp .Laddtosbuf
	.Lelsecondition: 
		addl $87, %r8d
	.Laddtosbuf: 
		movl %r8d, %esi //esi = 49
		shr $4, %r10d
		movl %r10d, %r8d
		inc %r9d
		addq $1, %rsi
		jmp .Ltopforloop
	.Lendforloop:
		movl $0, %r9d
		//jmp .Lreversestring
		//jmp .Lnullterminatestring
		movq %rsi, %rax 
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

/* vim:ft=gas:
 */
