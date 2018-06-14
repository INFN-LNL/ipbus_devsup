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

PROD_SYS_LIBS += cactus_uhal_uhal
PROD_SYS_LIBS += cactus_uhal_grammars
PROD_SYS_LIBS += cactus_uhal_log
#iocAsyn_LIBS += boost_thread
#iocAsyn_LIBS += boost_thread
PROD_SYS_LIBS += boost_filesystem
PROD_SYS_LIBS += boost_program_options
PROD_SYS_LIBS += boost_unit_test_framework
#iocAsyn_LIBS += pthread

PROD_SYS_LIBS += cactus_uhal_tests 
PROD_SYS_LIBS += boost_chrono 
PROD_SYS_LIBS += boost_regex 
PROD_SYS_LIBS += boost_system 
#iocAsyn_LIBS += pugixml 

#PROD_IOC = testioc
#testioc_SRCS += test_bacco_ipbus.cpp
#testioc_SRCS += registerIPBUS_gcu.cpp
#testioc_SRCS += myUhal.cpp
#testioc_SRCS += gcu.cpp
#testioc_SRCS += gcuLibr
#IPBUS libraries
#SHRLIB_SEARCH_DIRS += /opt/cactus/lib
#testioc_LIBS += ioc
#testioc_LIBS += cactus_uhal_uhal
#testioc_LIBS += cactus_uhal_grammars
#testioc_LIBS += cactus_uhal_log
#testioc_LIBS += boost_thread
#testioc_LIBS += boost_filesystem
#testioc_LIBS += boost_thread
#testioc_LIBS += boost_program_options
#testioc_LIBS += boost_unit_test_framework

#testioc_LIBS += cactus_uhal_tests 
#testioc_LIBS += boost_chrono 
#testioc_LIBS += boost_regex 
#testioc_LIBS += boost_system 
#testioc_LIBS += pugixml 

# PROD_IOC = testboost
# testboost_SRCS += test_boost.cpp
# testboost_LIBS += cactus_uhal_uhal
# testboost_LIBS += cactus_uhal_grammars
# testboost_LIBS += cactus_uhal_log
# testboost_LIBS += boost_thread
# testboost_LIBS += boost_filesystem
# testboost_LIBS += boost_thread
# testboost_LIBS += boost_program_options
# testboost_LIBS += boost_unit_test_framework
# #testioc_LIBS += pthread

# testboost_LIBS += cactus_uhal_tests 
# testboost_LIBS += boost_chrono 
# testboost_LIBS += boost_regex 
# testboost_LIBS += boost_system

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
