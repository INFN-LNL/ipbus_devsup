#include <stdio.h>
#include <sys/types.h>
#include <string>

#include "uhal/uhal.hpp"

using namespace uhal;


int main(int argc, const char **argv)
{
	if (argc != 3) {
		printf("write the right number of arguments:\n");
		printf("\t the path to connection file, i.e. : path/to/conn.xml\n");
		printf("\t the id of the ioc epics\n");
		return 0;
	}
    


    // for (int i = 0; i < argc; ++i) {
    //     std::cout << argv[i] << std::endl;
    // }


    char* _file1 = "file://"; char *file1= (char *)malloc(strlen(_file1)+strlen(argv[1])+1);
    strcpy(file1, _file1 ); strcat(file1, argv[1]);
    ConnectionManager manager(file1);
    HwInterface hw = manager.getDevice(argv[2]);


    ValWord< uint32_t > reg = hw.getNode ( "sl1" ).getNode("FMC_CTRL_REG").read();
    hw.dispatch();

    std::cout << "REG = " << reg.value() << std::endl;
    
	return 0;
}
