/* Assembly language implementation of hexdump functions */

	.section .rodata

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
		ret
		//push % ebp
		//movl %esp, %ebp

		//subl $4, %esp //make room for dec
		//movzx 8(%ebp), %esp //move int casted byteval into dec
		//subl $4, %esp //make room for div
		//sub1 $4, %esp //make room for mod
		
	hex_format_offset:
		ret

/* vim:ft=gas:
 */
