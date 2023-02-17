/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */
	hex_to_printable:	
		push % ebp
		movel %esp, %ebp

		subl $4, %esp //make room for dec
		
		movzx 8(%ebp), %esp //move int casted byteval into dec
		cmpq $32, %esp
		jl .non_printable //jump if less than 32
		cmpq $126, %esp
		jg .non_printable //jump if greater than 126
		move 8(%esp), %eax
		jmp .done
	.non_printable: 
		mov 46, %eax //is this allowed
		jmp .done
	.done: 
		addl $4, %esp 
		pop %ebp
		ret
		

	hex_format_byte_as_hex: 


/* vim:ft=gas:
 */
