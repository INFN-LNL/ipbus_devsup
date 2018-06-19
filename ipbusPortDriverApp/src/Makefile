TOP=../..

include $(TOP)/configure/CONFIG

#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

CXXFLAGS += -std=c++11

#==================================================
# build a support library

LIBRARY_IOC += iocAsyn


#rfc_SRCS += $(wildcard ./librfc/*.cpp)

# IPBUS headers
USR_INCLUDES += -I/opt/cactus/include/


# install rfc.dbd into <top>/dbd
DBD += iocAsyn.dbd


# specify all source files to be compiled and added to the library

iocAsyn_SRCS += iocSh.c
iocAsyn_SRCS += iocAsyn.c
iocAsyn_SRCS += baccoObj.cpp
iocAsyn_SRCS += ioCmd.cpp
iocAsyn_SRCS += baccoIo.cpp
iocAsyn_SRCS += filter.c

iocAsyn_LIBS += $(EPICS_BASE_IOC_LIBS)
iocAsyn_LIBS += ioc

USR_LDFLAGS += -L/opt/cactus/lib
USR_LDFLAGS += -Wl,-rpath,/opt/cactus/lib
iocAsyn_SYS_LIBS += cactus_uhal_grammars
iocAsyn_SYS_LIBS += cactus_uhal_log
iocAsyn_SYS_LIBS += cactus_uhal_uhal
iocAsyn_SYS_LIBS += cactus_uhal_tests 

PROD_IOC = testreading
testreading_SRCS += test_reg_reading.cpp
testreading_LIBS += iocAsyn
testreading_LIBS += asyn
testreading_SYS_LIBS += cactus_uhal_uhal
testreading_SYS_LIBS += boost_system 


include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE


#librfc.a:
#	$(MAKE) -C ../librfc $@
#	cp ../librfc/$@ .    
#
#cleanlibrfc:
#	$(MAKE) -C ./librfc clean
#	
#clean:: cleanlibrfc
