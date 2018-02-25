#!/bin/sh

CC_COMPILER=$1

export CC=$CC_COMPILER

if [ $CC = "clang" ]
then
    export CXX="clang++"
    STD_LIB_OPTION="-DGE_USE_LIBC++=OFF"
else
    export CXX="g++"
fi

set -e

conan remote add common https://api.bintray.com/conan/ivanovnikita/common --insert
conan remote add graphics https://api.bintray.com/conan/ivanovnikita/graphics_engine --insert

#export VK_LOADER_DEBUG=all
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release $STD_LIB_OPTION ..
make
cd bin
./graphics_engine-common-test
#./graphics_engine-graphics-test
