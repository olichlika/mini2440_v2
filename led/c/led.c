int main(){
	unsigned int *pGPBCON = (unsigned int *)0x56000010;
	unsigned int *pGPBDAT = (unsigned int *)0x56000014;
	*pGPBCON = 0x00015400;
	*pGPBDAT = 0x00000080;
	return 0;
}