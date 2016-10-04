###############################################################################
######################## Copyright 2016 Marleson Graf #########################
############################ <aszdrick@gmail.com> #############################
###############################################################################

#!/bin/sh

echo "-------------------------------------------------------------"
echo "This script will download and install all GTK dependencies"
echo "to compile windows applications using MinGW for x86-64"
echo "-------------------------------------------------------------"

root=$PWD
version=$1
if [ "$version" != "win32" ] && [ "$version" != "win64" ]; then
    echo
    echo "You can pass the desired version [win32 or win64]"
    echo "Assuming win64..."
    echo
    version="win64"
fi
if ! [ -f gtk+-bundle_3.10.4-20131202_$version.zip ]; then
    wget http://win32builder.gnome.org/gtk+-bundle_3.10.4-20131202_$version.zip
fi
sudo mkdir -p /opt/crosstools/gtk_$version
sudo unzip gtk+-bundle_3.10.4-20131202_$version.zip -d /opt/crosstools/gtk_$version
cd /opt/crosstools/gtk_$version
find -name '*.pc' | while read pc; do\
 sudo sed -e "s@^prefix=.*@prefix=$PWD@" -i "$pc";\
done
cd $root
mkdir -p external/libraries/windows
cp /opt/crosstools/gtk_$version/bin/*.dll external/libraries/windows

echo "-------------------------------------------------------------"
echo "Notice: in order to compile the source code, you will need to"
echo "set PKG_CONFIG_PATH:=/opt/crosstools/gtk_deps/lib/pkgconfig"
echo "-------------------------------------------------------------"
