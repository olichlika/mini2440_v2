.text
.global _start
_start:
	b reset
	ldr pc, =do_und

do_und:
	//设置栈
	ldr sp, =0x34000000
	//保存现场
	stmdb sp!, {r0-r12, lr}
	
	//处理异常
	mrs r0, cpsr
	mrs r1, spsr
	ldr r2, =und_string
	bl PrintException
	
	//恢复现场
	ldmia sp!, {r0-r12, pc}^ 
	
und_string:
	.string "undefined instruction exception"
.align 4 //保证4字节对齐
	
reset:
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
	mov r1, #0
	ldr r2, =_start //第一条指令地址
	ldr r3, =__bss_start //bss段起始地址
cpy:
	ldr r4, [r1] //从data_load_addr拷贝1个字节 效率低
	str r4, [r2]
	add r1, r1, #4
	add r2, r2, #4
	cmp r2, r3
	ble cpy //小于
	
	//清除bss段
	ldr r1, =__bss_start 
	ldr r2, =_end
	mov r3, #0
clean:	
	str r3, [r1]
	add r1, r1, #4
	cmp r1, r2
	ble clean
	
	ldr pc, =sdram //绝对跳转，到sdram中
sdram:
	//初始化串口
	bl Init_UART0
	
	bl print1
	
und_code:
	.word 0x03000000//故意加入一条未定义指令
	bl print2
	
	//****调用main****
	ldr pc, =main
halt:
	b halt