The `registers.xml` file represents the map of the FPGA registers. It should not be changed. Mostly, a change to this file implies an edit to the firmware, but changes not affecting the firmware are still possible. 

The nodes with **id** equals to **stat_reg, sl1, sl2** must not be changed!

The `connections.xml` file defines the number of connections and their names, and links the target IP to the correct registers configuration.