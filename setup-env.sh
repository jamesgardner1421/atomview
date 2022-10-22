# Copyright (c) 2020 LunarG, Inc.

# source this file into an existing shell.

VULKAN_SDK="$HOME/apps/VulkanSDK/macOS"
export VULKAN_SDK
PATH="$PATH:$VULKAN_SDK/bin"
export PATH
DYLD_LIBRARY_PATH="$VULKAN_SDK/lib:${DYLD_LIBRARY_PATH:-}"
export DYLD_LIBRARY_PATH
echo "This script is sets both VK_ADD_LAYER_PATH and VK_LAYER_PATH"
VK_LAYER_PATH="$VULKAN_SDK/share/vulkan/explicit_layer.d"
export VK_LAYER_PATH
VK_ADD_LAYER_PATH="$VULKAN_SDK/share/vulkan/explicit_layer.d"
export VK_ADD_LAYER_PATH
VK_ICD_FILENAMES="$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json"
export VK_ICD_FILENAMES
VK_DRIVER_FILES="$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json"
export VK_DRIVER_FILES
