#!/bin/sh

set -e

conan remote add common https://api.bintray.com/conan/ivanovnikita/common --insert
conan remote add graphics https://api.bintray.com/conan/ivanovnikita/graphics_engine --insert

CMAKE_VERSION=3.10.0
CMAKE_VERSION_DIR=v3.10
CMAKE_OS=Linux-x86_64

CMAKE_TAR=cmake-$CMAKE_VERSION-$CMAKE_OS.tar.gz
CMAKE_URL=http://www.cmake.org/files/$CMAKE_VERSION_DIR/$CMAKE_TAR
CMAKE_DIR=$TRAVIS_BUILD_DIR/cmake-$CMAKE_VERSION

wget --quiet $CMAKE_URL
mkdir -p $CMAKE_DIR
tar --strip-components=1 -xzf $CMAKE_TAR -C $CMAKE_DIR 
export PATH=$CMAKE_DIR/bin:$PATH

#export VK_LOADER_DEBUG=all
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd bin
./graphics_engine-common-test
#./graphics_engine-graphics-test
