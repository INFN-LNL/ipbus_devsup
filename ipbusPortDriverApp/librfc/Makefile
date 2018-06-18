#
#OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
#
#LIBS := -l usb-1.0
#CXXFLAGS += -g -Wall -fPIC
#
#all: librfc.so librfc.a
#
#
#librfc.a: $(OBJS)
#	$(AR) rcs $@ $?
#
#librfc.so: $(OBJS)
#	$(CXX) $(CXXFLAGS) --shared -o $@ $? $(LIBS)
#
#clean:
#	-rm -rf *.o *.so *.a

TOP=../..

include $(TOP)/configure/CONFIG

CXXFLAGS += -std=c++11

LIBRARY_IOC += ioc

# IPBUS headers
USR_INCLUDES += -I/opt/cactus/include/


ioc_SRCS += baccoObject.cpp
ioc_SRCS += gpioBase.cpp
ioc_SRCS += register32.cpp
ioc_SRCS += registerIPBUS.cpp
ioc_SRCS += uhalWrap.cpp

INC += baccoObject.h
INC += gpioBase.h
INC += register32.h
INC += registerIPBUS.h
INC += uhalWrap.h


USR_LDFLAGS += -L/opt/cactus/lib
USR_LDFLAGS += -Wl,-rpath,/opt/cactus/lib

ioc_SYS_LIBS += cactus_uhal_uhal
ioc_SYS_LIBS += cactus_uhal_grammars
ioc_SYS_LIBS += cactus_uhal_log
ioc_SYS_LIBS += boost_thread
ioc_SYS_LIBS += boost_filesystem
ioc_SYS_LIBS += boost_thread
ioc_SYS_LIBS += boost_program_options
ioc_SYS_LIBS += boost_unit_test_framework
#ioc_SYS_LIBS += pthread

ioc_SYS_LIBS += cactus_uhal_tests 
ioc_SYS_LIBS += boost_chrono 
ioc_SYS_LIBS += boost_regex 
ioc_SYS_LIBS += boost_system 
#ioc_SYS_LIBS += pugixml 


include $(TOP)/configure/RULES
