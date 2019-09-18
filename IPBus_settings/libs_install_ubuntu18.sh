echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/cactus/lib" >> ~/.bashrc
echo "export PATH=/opt/cactus/bin:$PATH" >> ~/.bashrc
apt install libpugixml-dev erlang
sudo mkdir /opt/cactus
cd /opt/cactus
sudo git clone --depth=1 -b v2.6.4 https://github.com/ipbus/ipbus-software.git
cd ipbus-software
sudo make
sudo make install