.text
.global _start
_start:
	ldr r0, =0x56000010
	mov r1, #0x00015400
	str r1, [r0]

	ldr r0, =0x56000014
	ldr r1, =0x00000040
	str r1, [r0]
halt:
	b halt