# Instruction to create an EPICS IOC that uses IPBus.

1.  Install fundamental libraries to execute ipbus. Look at **libs_installation.sh**.

2.  Copy the **file_connections.xml** and the **file_regs.xml**, located inside this folder, in your EPICS IOC (possibly in the same folder of your st.cmd files).
    Edit the following values in the **file_connections.xml**:

    - the tag **id**: Edit this with a suitable name for your hardware.
    - the IP address in the **uri** tag: Associate the instrument with the correct IP address given to the instrument. (To associate the MAC address to an IP address see the rarp_instruction.md file).
    - the tag **address_table**: Set the path to the **file_regs.xml**.
      Notes:
    - To edit the port value in the **uri** tag a firmware update is required. (Don't change this value - Default is 50001)

3.  In the st.cmd file add the line:

          baccoCreateIPBusPort <IPBUS_PORT_NAME> <PATH_TO_CONNECTION_XML> <IOC_CONNECTION_NAME>

    Where:

    - _baccoCreateIPBusPort_ : it is the command to create the hardware connections.
    - _IPBUS_PORT_NAME_ : it is the port name to assign.
    - _PATH_TO_CONNECTION_XML_ : it is a string (between '"') with the path to the **file_connections.xml** file.
    - _IOC_NAME_ : it is the ID pointing to the hardware IOC. It is the tag ID in the **file_connections.xml**.

4.  The **file_regs.xml** file represents the map of the FPGA registers. A change to this file implies an edit to the firmware (Don't change this file). Main nodes (id = stat_reg, sl1, sl2) must not be changed. Changes not affecting the firmware are still possible.

5.  A PV with the INP or OUT field can read information from one of the addresses specified in the **file_regs.xml** file. Here an example:

          record(ai, "Example1") {
             field(INP, "@asyn(IPBUS_PORT_NAME,0,0)reg(1,0x58,1,3,u)")
          }

    In this case this PV is connected to <node id="BPM_CTRL_REG" address="0x58"/>.
    The first parameter in the _reg function_ must be 1 which refers to the main node _sl1_ and the second parameters refers to the register address in hexadecimal value.

    Another example is:

          record(waveform, "Example2") {
             field(INP, "@asyn(IPBUS_PORT_NAME,0,0)regArray(2,0,512)")
          }

    In this case the PV is connected to <node id="reg_0"   address="0x000" mode="block" size="0x200"/>.
    The first parameter in the _regArray function_ must be 2 which refers to the main node _sl2_, the second parameters refers to the register address (0 because the node is made up of a unique block) and the third value refers to the block size in decimal value. _regArray_ reads node in **block** mode

    We can't associate a PV to a non existing node.
