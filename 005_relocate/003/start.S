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

	/*设置CLKDIVN*/
	ldr r0, =0x4C000014
	ldr r1, =0x5
	str r1, [r0]

	/*设置CPU异步模式*/
	mrc p15,0,r0,c1,c0,0 
	orr r0,r0,#0xc0000000 /*R1_nF:OR:R1_iA */
	mcr p15,0,r0,c1,c0,0

	/*设置MPLLCON*/
	ldr r0, =0x4C000004
	ldr r1, =(92 << 12) | (1 << 4) | (1<<0)
	str r1, [r0]	


	//****设置栈****
	mov r1, #0
	ldr r0, [r1] /* 读出原来的值备份 */
	str r1, [r1] /* 0->[0] */ 
	ldr r2, [r1] /* r2=[0] */
	cmp r1, r2   /* r1==r2? 如果相等表示是NAND启动 */
	ldr sp, =0x40000000+4096 /* 先假设是nor启动 */
	moveq sp, #4096  /* nand启动 */
	streq r0, [r1]   /* 恢复原来的值 */
	
	//初始化sdram
	bl Init_SDRAM
	
	//****重定位****
	mov r1, #0x800
	ldr r0, [r1]
	mov r1, #0x30000000
	str r0, [r1]
	
	//****调用main****
	bl main
halt:
	b halt