.text
.global _start
_start:
	//****关闭看门狗****
	ldr r0, =0x53000000
	mov r1, #0
	str r1, [r0]

	//****设置栈****
	ldr sp, =4096 //nand启动 设置在4k顶部
	//ldr sp, =0x4000000 + 4096 //nor启动

	//****调用main****
	bl main
halt:
	b halt