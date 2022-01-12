[![pipeline status](https://baltig.infn.it/epicscs/ipbus_devsup/badges/master/pipeline.svg)](https://baltig.infn.it/epicscs/ipbus_devsup/commits/master)

# IPbus support for BACCO

## Pre-requisites

This module requires the uhal library. This library has been tested on Ubuntu (16.04, 18.04, 20.04) and CentOS 7. 

On centOS we can install the uhal library from the official repository; on Ubuntu we have to download and compile the source files from the fit repository.

The main difference between the listed ubuntu versions is the packages installation. The remaining instructions are equals.

### Ubuntu

The packages required by Ubuntu to download and the compile from the source files uhal and to download and compile the EPICS module are:
- git
- make
- gcc-c++
- python (2.7 for Ubuntu16.04 and Ubuntu18.04 and 3.x for Ubuntu20.04)
- libreadline-dev
- boost
- pugixml
- erlang

On Ubuntu 20.04:

```
apt-get update
apt-get install -y build-essential git python-is-python3 libreadline-dev
apt-get install -y libboost-all-dev libpugixml-dev erlang
```

On Ubuntu18.04:

```
apt-get update
apt-get install -y build-essential git python libreadline-dev
apt-get install -y libboost-all-dev libpugixml-dev erlang
```

On Ubuntu16.04 we need to create two files to fix a bug on the erlang library:

```
apt-get update
apt-get install -y build-essential git python-is-python3 libreadline-dev
apt-get install -y libboost-all-dev libpugixml-dev erlang
touch /usr/lib/erlang/man/man1/x86_64-linux-gnu-gcov-tool.1.gz
touch /usr/lib/erlang/man/man1/gcov-tool.1.gz
```

Then we download the source files in the `/opt/cactus/ipbus-software` directory and we compile them. We only compile the `uhal` library.
The following command may require to be executed using sudo.

```
mkdir /opt/cactus
git clone --depth=1 -b v2.8.1 --recurse-submodules https://github.com/ipbus/ipbus-software.git /opt/cactus/ipbus-software
make -C /opt/cactus/ipbus-software Set=uhal
make install -C /opt/cactus/ipbus-software Set=uhal
```

### CentOS7

The CentOS packages required to install uhal, and download and compile the EPICS modules are:
- gcc-c++
- git
- make
- readline-devel

```
yum -y install gcc-c++ git make readline-devel
```

Download the appropriate YUM repository file from the ipbus repository, copy to /etc/yum.repos.d/ and rename to ipbus-sw.repo. Then install the software.

```
curl https://ipbus.web.cern.ch/doc/user/html/_downloads/ipbus-sw.centos7_gcc8.x86_64.repo -o /etc/yum.repos.d/ipbus-sw.repo
yum clean all
yum -y groupinstall uhal
```

# How to use

## Update the EPICS IOC configuration

If using epicsmng, add to your epicsmng `user.settings` file the following lines:

```
ipbus_url+="https://baltig.infn.it/epicscs/ipbus_devsup.git"
ipbus_requires+="base asyn"
ipbus_optionas+=
ipbus_dbd+="iocAsyn.dbd"
ipbus_libs+="iocAsyn"
```

If the file does not exist create it following the instructions on epicsmng.

Add to the _.conf_ file in your ioc the line: `ipbus = <TAG>` where _TAG_ refers to the version (or branch) you want to use.

Otherwise add it to your IOC as you normally do with other epics modules.

To correctly use the library we need to update some environment variables. 

```
export LD_LIBRARY_PATH=/opt/cactus/lib:$LD_LIBRARY_PATH
export PATH=/opt/cactus/bin:$PATH
```

These commands take effect only on the current terminal. Add them to your `~/.bashrc` file to keep them permanent.

    If you need to start the ioc as a service, add the LD_LIBRARY_PATH to the environment variable of the service.

## Load the registers configuration in the EPICS IOC

1. Associate the MAC addresses of the instruments to control to the correct IP as described [here](utilities/rarp/README.md).
2. Copy `connections.xml` and `registers.xml` files (placed in `utilities/epics_ioc_integration/`) in your EPICS IOC. The location is not mandatory but we suggest to copy them in the same folder of your `st.cmd` file. 
2. Edit `connections.xml`:
   - Edit the value of the tag `id` with a suitable name for your hardware. (i.e. id="MyIoc")
   - Associate the tag `uri` to the correct IP address. ("ipbusudp-2.0://10.10.10.101:50001"). Do not change the port number, it must be **50001**. To edit the port value a firmware update is required.
   - If the `registers.xml` file is not placed in the same folder of `connections.xml`, update the value of the tag `address_table` with the path to the `registers.xml` file.
3. In the `st.cmd` file create an IPBUS port for each instrument to control. The syntax is: `baccoCreateIPBusPort <IPBUS_PORT_NAME> <PATH_TO_CONNECTION_XML> <IOC_CONNECTION_NAME>`.
    - **baccoCreateIPBusPort** is the command to create the IPBUS port.
    - **IPBUS_PORT_NAME** is the port name to use in the IOC records.
    - **PATH_TO_CONNECTION_XML** is the path to the `connection.xml` file.
    - **IOC_NAME** is the ID of the connection defined in the `connections.xml` file.

## Create PV records using the IPBUS protocol

A PV with an INP field can receive data from one of the IPBUS port defined in the `st.cmd` file. Similarly a PV with an OUT field can send data to one of them. 

The syntax to use in the INP or OUT field is 

`asyn(IPBUS_PORT_NAME,0,0)reg(REG_FUNC,ADDRESS[,WIDTH[,SHIFT[,SIGNED[,DEC_BITS]]]])`
- **IPBUS_PORT_NAME** is the IPBUS port name defined in the `st.cmd` file.
- **REG_FUNC** is the _register function_:
  - 1 = _sl1_.
  - 2 = _sl2_.
  In this case it is always 1.
- **ADDRESS** is the _register address_, of the specified _register function_, in hexadecimal value.
- **WIDTH** is the number of bits, of the read value, to consider.
- **SHIFT** is the number of position to right shift the read value.
- **SIGNED** specifies if the acquired value is signed or unsigned.
- **DEC_BITS** 

      record(ai, "Example1") {
            field(DTYP, "asynInt32")
            field(INP,  "@asyn("myIoc",0,0)reg(1,0x58,1,3,u)")
      }

      In this case, Example1 is connected to <node id="BPM_CTRL_REG" address="0x58"/> in read-only mode. More in details this PV reads only the the third bit of the acquired value.

Or, to read nodes in block mode,

`asyn(IPBUS_PORT_NAME,0,0)regArray(REG_FUNC,START_ADD,WIDTH)`
- **IPBUS_PORT_NAME** is the IPBUS port name defined in the `st.cmd` file.
- **REG_FUNC** is the _register function_:
  - 1 = _sl1_.
  - 2 = _sl2_.
  In this case it is always 2.
- **START_ADD** is the start address. Always 0 because the second node is made up of a unique block.
- **WIDTH** is the number of registers to read.

      record(waveform, "Example2") {
            field(DTYP, "asynInt32ArrayIn")
            field(INP, "@asyn("myIoc",0,0)regArray(2,0,512)")
      }

    In this case the PV is connected to <node id="reg_0"   address="0x000" mode="block" size="0x200"/>.

It is not possible to associate a PV to a non existing node!

# Developers

Install and compile from this repository

```
git clone https://baltig.infn.it/epicscs/ipbus_devsup.git
cd ipbud_devsup
epicsmng makemodules ipbus_mod.conf
make
```
