# From your user's home
# Install ControlHub
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/cactus/lib" >> ~/.bashrc
echo "export PATH=/opt/cactus/bin:$PATH" >> ~/.bashrc
su
cd /opt
mkdir cactus
cd cactus
sudo yum -y install epel-release
sudo yum -y install make rpm-build git-core erlang gcc-c++ boost-devel pugixml-devel python-devel
git clone --depth=1 -b v2.6.1  https://github.com/ipbus/ipbus-software.git
cd ipbus-software
make
sudo make install