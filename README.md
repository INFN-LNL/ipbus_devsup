[![pipeline status](https://baltig.infn.it/epicscs/ipbus_devsup/badges/master/pipeline.svg)](https://baltig.infn.it/epicscs/ipbus_devsup/commits/master)

# IPbus support for bacco

## Pre-requisites

### Ubuntu 18.04/20.04

1. Install ipbus dependencies

    The essential dependencies are:
    - git
    - make
    - c++ compiler
    - python (2 or 3)
    - ...
    
    ```
    sudo apt install -y build-essential git python-is-python3
    ```

    The ipbus dependencies are:
    - boost
    - pugixml
    - erlang

    ```
    sudo apt install -y libboost-all-dev libpugixml-dev erlang 
    ```

2. Install and compile ipbus

    ```
    sudo mkdir /opt/cactus
    cd /opt/cactus
    sudo git clone --depth=1 -b v2.8.1 https://github.com/ipbus/ipbus-software.git
    cd ipbus-software
    sudo make Set=uhal
    sudo make install Set=uhal
    ```

### CentOS7

1. Install ipbus dependencies

    The essential dependencies are:
    - git
    - make
    - c++ compiler
    - python (2 or 3)
    - ...

    ```
    sudo yum -y install epel-release make rpm-build git-core  gcc-c++ python-devel 
    ```

    The ipbus dependencies are:
    - boost
    - pugixml
    - erlang


    ```
    sudo yum -y install boost-devel pugixml-devel erlang
    ```

2. Install and compile ipbus

    ```
    sudo mkdir /opt/cactus
    cd /opt/cactus
    sudo git clone --depth=1 -b v2.8.1 https://github.com/ipbus/ipbus-software.git
    cd ipbus-software
    sudo make Set=uhal
    sudo make install Set=uhal
    ```

3. Set the environment variables:

    ```
    echo "export LD_LIBRARY_PATH=/opt/cactus/lib:$LD_LIBRARY_PATH" >> ~/.bashrc
    echo "export PATH=/opt/cactus/bin:$PATH" >> ~/.bashrc
    ```


## Installation

Add to your epicsmng `user.settings` file the following lines:

```
ipbus_url+="https://baltig.infn.it/epicscs/ipbus_devsup.git"
ipbus_requires+="base asyn"
ipbus_optionas+=
ipbus_dbd+="iocAsyn.dbd"
ipbus_libs+="iocAsyn"
```

If the file does not exist create it following the instructions on epicsmng.

Add to the target IOC's epicsmng configuration file (for example myIoc.conf) the following line: 

```
ipbus = master
```

## How to use

TODO

If you need to start the ioc as a service, add the `LD_LIBRARY_PATH` to the environment variable of the service.

## Developers

Requirements:
- epicsmng

Installation:
```
git clone https://baltig.infn.it/epicscs/ipbus_devsup.git
cd ipbud_devsup
epicsmng makemodules ipbus_mod.conf
make
```
