.text
.global _start
_start:
	//****关闭看门狗****
	ldr r0, =0x53000000
	mov r1, #0
	str r1, [r0]

	//****设置MPLL时钟****
	//设置LOCKTIME
	ldr r0, =0x4C000000
	ldr r1, =0xFFFFFFFF
	str r1, [r0]

	//设置CLKDIVN
	ldr r0, =0x4C000014
	ldr r1, =0x5
	str r1, [r0]

	//设置CPU异步模式
	mrc p15,0,r0,c1,c0,0 
	orr r0,r0,#0xc0000000 //R1_nF:OR:R1_iA 
	mcr p15,0,r0,c1,c0,0

	//设置MPLLCON
	ldr r0, =0x4C000004
	ldr r1, =(92 << 12) | (1 << 4) | (1<<0)
	str r1, [r0]	


	//****设置栈****
	ldr sp, =4096 //nand启动 设置在4k顶部
	//ldr sp, =0x4000000 + 4096 //nor启动

	//****调用main****
	bl main
halt:
	b halt