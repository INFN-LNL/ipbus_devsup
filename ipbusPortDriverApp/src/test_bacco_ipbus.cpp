#include <stdio.h>
#include <sys/types.h>
#include <string>
#include <uhal/uhal.hpp>

using namespace uhal;

int main(int narg, const char **argv)
{

    ConnectionManager manager ( "file://ioc_connections.xml" );

	HwInterface hw=manager.getDevice ( "IOC" );
	return 0;
	ValWord< uint32_t > reg = hw.getNode("stat_reg").getNode("magic_world").read();

	hw.dispatch();

	std::cout << "B.A = " << reg.value() << std::endl;

//	myUhal m(connection1);
//
//
//
//	uint32_t x = 7;
//	uint32_t a = 0;
//	registersIPBUS_gcu gcu_reg(&m, id1, "reg_32");
//	gcu gcu_obj(&gcu_reg);
//
//	gcu_obj.setX(a,x);
//	std::cout << "X = " << gcu_obj.getX(a) << std::endl;
	return 0;
}
