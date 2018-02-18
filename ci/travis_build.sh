#!/bin/sh

set -e

conan remote add common https://api.bintray.com/conan/ivanovnikita/common --insert
conan remote add graphics https://api.bintray.com/conan/ivanovnikita/graphics_engine --insert

mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DGE_BUILD_GRAPHICS_TESTS=OFF ..
make
./bin/graphics_engine-common-test
