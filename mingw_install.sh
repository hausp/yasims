###############################################################################
######################## Copyright 2016 Marleson Graf #########################
############################ <aszdrick@gmail.com> #############################
###############################################################################

#!/bin/sh

ARCH=$(cat /etc/*release | grep ^NAME= | sed -r s/NAME=\"\(.+\)\"/\\1/)

if [ "$ARCH" == "Arch Linux" ]; then
    sudo pacman -S mingw-w64-gcc mingw-w64-binutils mingw-w64-crt \
    mingw-w64-headers mingw-w64-winpthreads
elif [ "$ARCH" == "Ubuntu" ]; then
    sudo apt-get install mingw-w64
fi
