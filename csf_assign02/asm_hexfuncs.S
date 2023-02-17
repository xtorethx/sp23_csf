/* Assembly language implementation of hexdump functions */

	.section .rodata
hex_string:

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

		//edi 0x48 72
		//rsi empty string

		movq $0, %rax
		movq $0, %rdx
		
		movw %di, %ax
		movb $16, %cl
		divb %cl 

		movzx %ah, %edx // edx = 8
		movzx %al, %eax // eax = 4
		
		movl %eax, 0(%rsi)
		movl %edx, 4(%rsi)
		
		ret
		
	hex_format_offset:
		ret

/* vim:ft=gas:
 */
