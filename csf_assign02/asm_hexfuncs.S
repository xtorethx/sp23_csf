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
		movq %rdi, %rsi // first arg (data_buf) to second arg
		movq $16, %rdx // set num chars read as 16
		movq $0, %rdi // 0 for read
		call read
		ret

	hex_write_string: 
		movq %rdi, %rsi //move s to second arg
	.Ltoploop:
		cmpb $0, (%rsi) // check that null terminator is not reached
		je .Lend // jump to end if so
		movl $1, %edi //first parameter
		andq $0xffff, %rdi //clear out potential garbage
		movl $1, %edx //third parameter
		andq $0xffff, %rdx //clear garbage
		call write
		addq $1, %rsi // increment to next char
		jmp .Ltoploop
	.Lend:
		ret
	
	hex_format_offset:
		movq %rdi, %r8 //offset (for mod)
		movq $0, %r9  //index counter
		movq %rdi, %r10 //offset (for div)
		addq $7, %rsi //start rsi from address of last element of sbuf
	.Ltopforloop:
		cmpq $8, %r9 //for loop check
		jge .Lendforloop
		andq $0xf, %r8 //offset mod 16
		movq $sHexDigits, %r11 //string of hex chars for conversion
		movb (%r8, %r11), %dl //corresponding hex char
		movb %dl, (%rsi) //move hex char into sbuf output string
		shr $4, %r10 //offset div 16
		movq %r10, %r8 //reset offset variable (offset = offset/16)
		inc %r9 //increment index tracker
		subq $1, %rsi //point to next char
		jmp .Ltopforloop
	.Lendforloop:
		movb $0, 9(%rsi) //null terminator
		movq %rsi, %rax //return sbuf
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

		movq %rsi, %rax // return formated sbuf
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
