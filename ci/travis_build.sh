#!/bin/sh

set -e

conan remote add common https://api.bintray.com/conan/ivanovnikita/common --insert
conan remote add graphics https://api.bintray.com/conan/ivanovnikita/graphics_engine --insert

#export VK_LOADER_DEBUG=all
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd bin
./graphics_engine-common-test
#./graphics_engine-graphics-test
