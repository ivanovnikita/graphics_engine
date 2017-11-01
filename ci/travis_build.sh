#!/bin/sh

set -e

VULKAN_VERSION="1.0.61.1"
mkdir vulkan
cd vulkan
wget -nv "https://vulkan.lunarg.com/sdk/download/$VULKAN_VERSION/linux/vulkansdk-linux-x86_64-$VULKAN_VERSION.run"
chmod ugo+x vulkansdk-linux-x86_64-$VULKAN_VERSION.run
./vulkansdk-linux-x86_64-$VULKAN_VERSION.run
cd "VulkanSDK/$VULKAN_VERSION"
source setup-env.sh

cd ../../..
mkdir build && cd build
cmake ..
make