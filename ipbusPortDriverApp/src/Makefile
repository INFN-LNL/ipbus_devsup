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

SHRLIB_SEARCH_DIRS += /opt/cactus/lib

iocAsyn_LIBS += cactus_uhal_uhal
iocAsyn_LIBS += boost_system 
iocAsyn_LIBS += cactus_uhal_tests 
iocAsyn_LIBS += cactus_uhal_log
iocAsyn_LIBS += boost_filesystem


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
