###################### Copyright (C) 2016 Marleson Graf #######################
######################### <github.com/aszdrick/mkm/> ##########################
############################ <aszdrick@gmail.com> #############################
###############################################################################
## Licensed under the Apache License, Version 2.0 (the "License");           ##
## you may not use this file except in compliance with the License.          ##
## You may obtain a copy of the License at                                   ##
##                                                                           ##
##     http://www.apache.org/licenses/LICENSE-2.0                            ##
##                                                                           ##
## Unless required by applicable law or agreed to in writing, software       ##
## distributed under the License is distributed on an "AS IS" BASIS,         ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  ##
## See the License for the specific language governing permissions and       ##
## limitations under the License.                                            ##
###############################################################################

#!/bin/sh

function extract_dlls() {
    mkdir -p external/libraries/windows
    cp /opt/crosstools/gtk_$version/bin/*.dll external/libraries/windows
}

echo "-------------------------------------------------------------"
echo "This script will download and install all GTK dependencies"
echo "to compile windows applications using MinGW for x86-64"
echo "-------------------------------------------------------------"

root=$PWD
v=$1
if [ "$v" != "win32" ] && [ "$v" != "win64" ]; then
    echo
    echo "You can pass the desired v [win32 or win64]"
    echo "Assuming win64..."
    echo
    v="win64"
fi
if ! [ -f gtk+-bundle_3.10.4-20131202_$v.zip ]; then
    wget http://win32builder.gnome.org/gtk+-bundle_3.10.4-20131202_$v.zip
fi
sudo mkdir -p /opt/crosstools/gtk_$v
sudo unzip gtk+-bundle_3.10.4-20131202_$v.zip\
 -d /opt/crosstools/gtk_$v
cd /opt/crosstools/gtk_$v
find -name '*.pc' | while read pc; do\
     sudo sed -e "s@^prefix=.*@prefix=$PWD@" -i "$pc";\
done
cd $root
extract_dlls

echo "-------------------------------------------------------------"
echo "Notice: in order to compile the source code, you will need to"
echo "set PKG_CONFIG_PATH:=/opt/crosstools/gtk_deps/lib/pkgconfig"
echo "-------------------------------------------------------------"
