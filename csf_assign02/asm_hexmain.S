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
main:
	/* TODO: add your code here */
	movq $data_buf, %rdi // move dat_buf base address to rdi
	call hex_read // call hex_read on data_buf
	movq %rdi, %r10 // save data_buf
	movl %eax, $r12d // move result of hex_read into r12
	xorl %r13d, %r13d // 0 out r13 (offset)
.LwhileLoop:
	cmpl $0, %r12d // compare 0 and the counter
	je .LwhileLoopDone // finish loop when 

	movl %r13d, %esi
	movq $sbuf, %rsi
	call hex_format_offset
	movq %rsi, %rdi // sbuf is changed to offset so write offset
	call hex_write_string
	movw $colon, %di // write :
	call hex_write_string
	movw $space, %di
	call hex_write_string
	xorq %r14, %r14 // 0 out loop counter
	xorq %r15, %r15 // 0 printable
	movq $printable, %r15 // move 16 empty printable string into r15 register

// %r14 is i
// %r12 is counter
.LloopTop:
	cmpl %r14d, %r12d
	jbe .LshortLine // line 30 in c_hexmain
	movq (%r10, $r14), %rdi // move data_buf[i] to first arg
	xorl %r11d, %r11d // 0 out tmp register
	movl %r11d, %esi // put in 2nd arg
	call hex_format_byte_as_hex
	movl %esi, %edi // move changed %esi to first arg
	call hex_write_string
	movw $space, %di
	call hex_write_string // write space
	movq (%r10, $r14), %rdi
	call hex_to_printable
	movq %rax, (%r15, %r14) // move result to printable[i]
	incq %r14

.LloopCond
	cmpq $16, %r14
	jb .LloopTop // jump back if loop counter < 16
	jmp .LwhileNextLine

.LshortLine
	movl $twospace, %edi
	call hex_write_string
	movw $space, %di
	call hex_write_string
	movq $0, (%r15, %r14) // null terminate at printable[i]
	incq %r14
	jmp .LloopCond

.LwhileNextLine
	movw $space, %di
	call hex_write_string // write space
	movq %r15, %rdi // printable to first arg
	call hex_write_string
	movw $newline, %di
	call hex_write_string // write newline
	addl %r12d, %r13d
	movq $data_buf, %rdi // move dat_buf base address to rdi
	call hex_read // call hex_read on data_buf
	movq %rdi, %r10 // save data_buf
	movl %eax, $r12d // move result of hex_read into r12 (counter)
	jmp .LwhileLoop

.LwhileLoopDone
	ret

/* vim:ft=gas:
 */
