#include <stdio.h>
#include <sys/types.h>
#include <string>
#include <uhal/uhal.hpp>
#include "uhalWrap.h"
#include "registerIPBUS.h"

using namespace uhal;

int main(int argc, const char **argv)
{
	if (argc != 3) {
		printf("write the right number of arguments:\n");
		printf("\t the path to connection file, i.e. : path/to/conn.xml\n");
		printf("\t the id of the ioc epics\n");
		return 0;
	}


    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }

    char* _file = "file://"; char *file= (char *)malloc(strlen(_file)+strlen(argv[1])+1);
    strcpy(file, _file ); strcat(file, argv[1]);

	hwIntWr hw(file, argv[2]);
	registersIPBUS sl1(&hw, "sl1");
	registersIPBUS sl2(&hw, "sl2");


	uint32_t reg_v; uint32_t val_w = 0x5e55f1ca;
	printf("\nTest memory space registers :\n");

	uint32_t add = 0x53;
	sl1.set(add, val_w);
	printf("reg[%03x] %08x\n", add, sl1.get(add));
	for(add = 0x58; add <= 0x5b; add++){
		sl1.set(add, val_w);
		printf("reg[%03x] %08x\n", add, sl1.get(add));
	}

	printf("\nTest bpm block memory registers :\n");
	uint32_t size = 0x200;
	uint32_t block[size];
	sl2.get(0,block,size);
	for(int i = 0; i < size; i++){
		if (i%0x4 > 0)
		    printf("\t mem[%03x] = %08x", i, block[i]);
		else
			printf("\n mem[%03x] = %08x", i, block[i]);
	}
	printf("\n");

	return 0;
}
