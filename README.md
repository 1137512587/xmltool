# xmltool
解压：
tar -xf libxml2-2.9.12.tar.xz
或
tar -zxvf libxml2-2.9.12.tar.gz
安装
./configure
cmake ./
make
make install

//查看安装库列表
pkg-config --list all

opensuse系统安装
zypper search | grep libxml2
zypper install libxml2

XML的数据结构
根元素