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

## Installation

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

## How to use

To correctly use the library we need to update some environment variables. 

```
export LD_LIBRARY_PATH=/opt/cactus/lib:$LD_LIBRARY_PATH
export PATH=/opt/cactus/bin:$PATH
```

These commands take effect only on the current terminal. Add them to your `~/.bashrc` file to keep them permanent.

If you need to start the ioc as a service, add the `LD_LIBRARY_PATH` to the environment variable of the service.

## Developers

Install and compile from this repository

```
git clone https://baltig.infn.it/epicscs/ipbus_devsup.git
cd ipbud_devsup
epicsmng makemodules ipbus_mod.conf
make
```
