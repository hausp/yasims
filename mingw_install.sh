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

ARCH=$(cat /etc/*release 2>/dev/null | grep ^NAME= \
     | sed -r s/NAME=\"\(.+\)\"/\\1/)

if [ "$ARCH" == "Arch Linux" ]; then
    sudo pacman -S mingw-w64-gcc mingw-w64-binutils mingw-w64-crt \
    mingw-w64-headers mingw-w64-winpthreads
    sudo ln -s /usr/bin/x86_64-w64-mingw32-g++ /usr/bin/mingw++
elif [ "$ARCH" == "Ubuntu" ]; then
    sudo apt-get install mingw-w64
    sudo ln -s /usr/bin/x86_64-w64-mingw32-g++ /usr/bin/mingw++
fi
