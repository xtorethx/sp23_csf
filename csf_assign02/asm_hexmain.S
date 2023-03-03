/* Hexdump main function, assembly language version */

	.section .data
data_buf: .space 16, 0
sbuf: .space 9, 0
hexchar: .space 3, 0
printable: .space 17, 0

	.section .rodata
colon: .string ":"
twospace: .string "  "
space: .string " "
newline: .string "\n"

	.section .text
	.globl main
	.globl hex_read
	.globl hex_write_string
	.globl hex_format_offset
	.globl hex_format_byte_as_hex
	.globl hex_to_printable
	
main:
	/* TODO: add your code here */
	movq $data_buf, %rdi // move dat_buf base address to rdi
	call hex_read // call hex_read on data_buf (line 23 in main)
	movq %rsi, %r13 // save data_buf
	movl %eax, %ebx // move result of hex_read into rbx; rbx = counter
	xorl %r12d, %r12d // 0 out r12 (offset)
.LwhileLoop:
	cmpl $0, %ebx // compare 0 and the counter; line 24 in main
	je .LwhileLoopDone // finish loop when 

	movl %r12d, %edi //moving offset into first param
	andq $0xffff, %rdi //clear garbage in rdi, only hold 16 bytes of offset
	movq $sbuf, %rsi //moving sbuf into second param
	call hex_format_offset // line 25 in main
	inc %rsi
	movq %rsi, %rdi // sbuf is changed to offset so write offset
	call hex_write_string //line 26 in main
	movq $colon, %rdi // write :
	call hex_write_string //line 27
	movq $space, %rdi
	call hex_write_string //line 28
	xorq %r14, %r14 // 0 out loop counter
	xorq %r15, %r15 // 0 printable
	movq $printable, %r15 // move 16 empty printable string into r15 register

// %r14 is i
// %rbx is counter
.LloopTop:
	cmpl %r14d, %ebx //check if stay in loop; line 30 in main
	jbe .LshortLine // line 30 in c_hexmain
	movb (%r13, %r14), %dil // move data_buf[i] to first arg
	andq $0xff, %rdi
	//xorl %r11d, %r11d // 0 out tmp register
	movq $hexchar, %rsi //put hexchar in second argument
	call hex_format_byte_as_hex //line 35
	movq %rsi, %rdi // move changed %esi to first arg
	call hex_write_string//line 36 in main
	movq $space, %rdi
	call hex_write_string // write space (line 37 in main)
	movq (%r13, %r14), %rdi //databuf[i]
	andq $0xff, %rdi
	call hex_to_printable
	movb %al, (%r15, %r14) // move result to printable[i] (line 38)
	andq $0xf, %rax
	incq %r14

.LloopCond:
	cmpq $16, %r14 //line 29 in main
	jb .LloopTop // jump back if loop counter < 16
	jmp .LwhileNextLine

.LshortLine:
	movq $twospace, %rdi
	call hex_write_string //line 31
	movq $space, %rdi
	call hex_write_string //line 32
	movb $0, (%r15, %r14) // null terminate at printable[i] (line 33 in main)
	incq %r14
	jmp .LloopCond

.LwhileNextLine:
	movq $space, %rdi
	call hex_write_string // write space
	movq %r15, %rdi // printable to first arg
	call hex_write_string
	movq $newline, %rdi
	call hex_write_string // write newline
	addl %ebx, %r12d//line 44 in main
	movq $data_buf, %rdi // move dat_buf base address to rdi
	call hex_read // call hex_read on data_buf
	movq %rdi, %r13 // save data_buf
	movl %eax, %ebx // move result of hex_read into rbx (counter)
	jmp .LwhileLoop

.LwhileLoopDone:
	ret

/* vim:ft=gas:
 */
