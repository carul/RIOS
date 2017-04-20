
struct VideoInformation
{
	int * Framebuffer;
	short Pitch;
	char ColorDepth;
	short Width;
	short Height;
};

void * FindMemoryAfterString(const char search[], long from, int bound=0x1000);

void _kernel_main(){
	void * memloc = FindMemoryAfterString("HIS_BGIN", 0x499);
	asm("mov rcx, %0\n\t"
		".l1:\n\t"
		"jmp .l1\n\t"
		:"=r"(memloc)
		);
}

void * FindMemoryAfterString(const char search[], long f, int bound)
{
	void * from = (void*)f;
	void * Addr = (void*)0x0;
	bool endof = false;
	int size = 0;
	for(int i = 0; i < bound; i++)
	{
		if(*((char*)from + i) == search[0])
		{
			void * TAddr = (void*)((char*)from+i);
			int arriter = 0;
			while(true)
			{
				if(search[i] == 0x0){
					Addr = TAddr;
					endof = true;
					break;
				}
				if(*((char*)from + arriter) == search[i]){
					size++;
					continue;
				}
				else
					break;
			}
		}
		if(endof)
			break;
	}
	return (void*)((char*)Addr+size);
}
