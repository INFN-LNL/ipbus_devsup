# Instruction to use IPBus with **RARP**

## CENTOS7

- Install rarpd-ss981107-42.el6.x86_64.rpm (yum does not provides this package)

- Create the **/etc/ethers** file and add the list of the couples (MAC Address - Hostname).
  Here an example: 01:02:03:04:05:06 iocXYZ

- If you don't have an OPNSense mapping the IP address with an hostname, create the **/etc/hosts** file and add the couples (IP Address - Hostname).
  Here an example: 192:168:10:10 iocXYZ

- Run **rarp** as a system service. You must specify the interface over which read the MAC addresses. Here an example of the command: `sudo rarpd -d eth0`
