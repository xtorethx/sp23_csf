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
		movw %di, %r8w //4 bytes from edi to r8d
		movw $0, %r9w  //r9w = 0, represents counter
		movw %di, %r10w //4 bytes from edi to r10w
	.Ltopforloop:
		cmpw $8, %r9w
		jge .Lendforloop
		andq $0xf, %r8 //r8d = 1
		movq $sHexDigits, %r11
		movb (%r8, %r11), %dl
		movb %dl, (%rsi)
		shr $4, %r10w
		movw %r10w, %r8w
		inc %r9w
		addq $1, %rsi
		jmp .Ltopforloop
	.Lendforloop:
		movw $0, (%rsi) //null terminator
		movw $0, %r9w //reset index
	.Ltopforlooprev:
		cmp $4, %r9w
		jge .Lnullterminatestring
		jmp .Lreversestring
	.Lreversestring:
		//fill this
		inc %r9w
		jmp .Ltopforlooprev
	.Lnullterminatestring:
		movw $0, (%rsi) //null terminator
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
