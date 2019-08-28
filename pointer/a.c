#include <stdio.h>

void test0(){
	char c;
	char *pc;
	char *pb;

	/*第一步 : 所有变量都保存在内存中，我们打印一下变量的存储地址*/
	printf("&c  =%p\n", &c);
	printf("&pc =%p\n", &pc);
	printf("&pb =%p\n", &pb);

	/*第二步：所有变量都可以保存某些值,接着赋值并打印*/
	c  = 'A';
	pc = &c;
	printf("c  =%c\n",c);
	printf("pc =%p\n",pc);

	/*第三步：使用指针：1)取值  2)移动指针*/
	printf("*pc =%c\n",*pc);	
	printf("//=================\n");			
}

void test1(){
	int  ia;
	int  *pi;
	char *pc;

	/*第一步 : 所有变量都保存在内存中，我们打印一下变量的存储地址*/
	printf("&ia =%p\n",&ia);
	printf("&pi =%p\n",&pi);	
	printf("&pc =%p\n",&pc);		

	/*第二步：所有变量都可以保存某些值,接着赋值并打印*/	
	ia = 0x12345678;
	pi = &ia;
	pc = (char *)&ia;
	printf("ia =0x%x\n",ia);	
	printf("pi =%p\n",pi);		
	printf("pc =%p\n",pc);

	/*第三步：使用指针：1)取值  2)移动指针*/
	printf("*pi =0x%x\n",*pi);			
	printf("pc =%p\t",pc);	printf("*pc =0x%x\n",*pc); pc=pc+1;
	printf("pc =%p\t",pc);	printf("*pc =0x%x\n",*pc); pc=pc+1;
	printf("pc =%p\t",pc);	printf("*pc =0x%x\n",*pc); pc=pc+1;
	printf("pc =%p\t",pc);	printf("*pc =0x%x\n",*pc);	
	printf("//=================\n");
}

void test2(){
	char ca[3]={'A','B','C'};
	char *pc;

	/*第一步 : 所有变量都保存在内存中，我们打印一下变量的存储地址*/
	printf("ca  =%p\n",ca);
	printf("&pc =%p\n",&pc);

	/*第二步：所有变量都可以保存某些值,接着赋值并打印*/
	//前面已经有ca[3]={'A','B','C'};
	pc = ca;
	printf("pc =%p\n",pc);

	/*第三步：使用指针：1)取值  2)移动指针*/
	printf("pc =%p\t",pc);	printf("*pc =0x%x\n",*pc); pc=pc+1;
	printf("pc =%p\t",pc);	printf("*pc =0x%x\n",*pc); pc=pc+1;
	printf("pc =%p\t",pc);	printf("*pc =0x%x\n",*pc);	
	printf("//=================\n");			
}

void test3(){
	int ia[3]={0x12345678,0x87654321,0x13572468};
	int *pi;

	/*第一步 : 所有变量都保存在内存中，我们打印一下变量的存储地址*/
	printf("ia  =%p\n",ia);
	printf("&pi =%p\n",&pi);

	/*第二步：所有变量都可以保存某些值,接着赋值并打印*/
	//前面已经有ia[3]={0x12345678,0x87654321,0x13572468};
	pi = ia;
	printf("pi =%p\n",pi);

	/*第三步：使用指针：1)取值  2)移动指针*/
	printf("pi =%p\t",pi);	printf("*pi =0x%x\n",*pi); pi=pi+1;
	printf("pi =%p\t",pi);	printf("*pi =0x%x\n",*pi); pi=pi+1;
	printf("pi =%p\t",pi);	printf("*pi =0x%x\n",*pi); 
	printf("//=================\n");	
}

void test4(){
 	char *pc="abc";
	/*第一步 : 所有变量都保存在内存中，我们打印一下变量的存储地址*/
	printf("&pc =%p\n",&pc);

	/*第二步：所有变量都可以保存某些值,接着赋值并打印*/
	//前面已经有pc="abc";
	
	/*第三步：使用指针：1)取值  2)移动指针*/
	printf("pc    =%p\n", pc);
	printf("pc+1    =%p\n", pc+1);
	printf("pc+2    =%p\n", pc+2);
	printf("*pc   =%c\n",*pc);
	printf("*(pc+1)   =%c\n",*(pc+1));
	printf("*(pc+2)   =%c\n",*(pc+2));
	printf("pc str=%s\n", pc);
}	

int main(){
	printf("sizeof(char   )=%d\n", sizeof(char   ));
	printf("sizeof(int    )=%d\n", sizeof(int    ));
	printf("sizeof(char  *)=%d\n", sizeof(char  *));
	printf("sizeof(char **)=%d\n", sizeof(char **));	
	printf("//=================\n");
	test4();
	return 0;
}