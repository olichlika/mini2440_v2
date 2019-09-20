.text
.global _start
_start:
	b reset //0x00
	ldr pc, und_addr //0x04
	b halt //0x08
	b halt //0x0c
	b halt //0x10
	b halt //0x14
	ldr pc, irq_addr //0x18
	
und_addr:
	.word do_und
	
irq_addr:
	.word do_irq
	
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

do_irq:
	//设置栈
	ldr sp, =0x33d00000
	//保存现场
	sub lr, lr, #4
	stmdb sp!, {r0-r12, lr}
	
	//处理异常
	bl handle_irq_c
	
	//恢复现场
	ldmia sp!, {r0-r12, pc}^ 
	
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
	// mov r1, #0
	// ldr r0, [r1] /* 读出原来的值备份 */
	// str r1, [r1] /* 0->[0] */ 
	// ldr r2, [r1] /* r2=[0] */
	// cmp r1, r2   /* r1==r2? 如果相等表示是NAND启动 */
	// ldr sp, =0x40000000+4096 /* 先假设是nor启动 */
	// moveq sp, #4096  /* nand启动 */
	// streq r0, [r1]   /* 恢复原来的值 */
	mov sp, #4096
	
	//初始化sdram
	bl Init_SDRAM
	
	//初始化Nand
	bl Init_Nand
	
	bl Init_UART0
	bl print1
	//****重定位****
	// mov r1, #0
	// ldr r2, =_start //第一条指令地址
	// ldr r3, =__bss_start //bss段起始地址
// cpy:
	// ldr r4, [r1] //从data_load_addr拷贝1个字节 效率低
	// str r4, [r2]
	// add r1, r1, #4
	// add r2, r2, #4
	// cmp r2, r3
	// ble cpy //小于
	
	mov r0, #0 //Nand地址
	ldr r1, =0x30000000	//目的地址
	ldr r2, =20480 //复制长度
	bl Nand_Copy_to_SDRAM 
	bl print2
	//清除bss段
	ldr r1, =__bss_start 
	ldr r2, =_end
	mov r3, #0
clean:	
	str r3, [r1]
	add r1, r1, #4
	cmp r1, r2
	ble clean
	
	//切换到usr模式
	mrs r0, cpsr         /* 读出cpsr */
	bic r0, r0, #0xf     /* 修改M4-M0为0b10000, 进入usr模式 */
	bic r0, r0, #(1<<7)  /* 清除I位, 使能中断 */
	msr cpsr, r0

	/* 设置 sp_usr */
	ldr sp, =0x33f00000
	
	ldr pc, =sdram //绝对跳转，到sdram中
sdram:
	//初始化串口
	//bl Init_UART0
	
	bl print1
	
und_code:
	.word 0x03000000//故意加入一条未定义指令
	//bl print2
	
	//****调用main****
	ldr pc, =main
halt:
	b halt