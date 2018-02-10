#!/bin/sh

set -e

conan remote add common https://api.bintray.com/conan/ivanovnikita/common --insert
conan remote add graphics https://api.bintray.com/conan/ivanovnikita/graphics_engine --insert

VULKAN_VERSION="1.0.61.1"
mkdir vulkan
cd vulkan
wget -nv "https://vulkan.lunarg.com/sdk/download/$VULKAN_VERSION/linux/vulkansdk-linux-x86_64-$VULKAN_VERSION.run"
chmod ugo+x vulkansdk-linux-x86_64-$VULKAN_VERSION.run
./vulkansdk-linux-x86_64-$VULKAN_VERSION.run
cd "VulkanSDK/$VULKAN_VERSION"
export VULKAN_SDK=$PWD/x86_64
export PATH=$VULKAN_SDK/bin:$PATH
export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH
export VK_LAYER_PATH=$VULKAN_SDK/etc/explicit_layer.d

cd ../../..
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./bin/graphics_engine-test
