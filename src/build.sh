#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: $0 [Path]";
  exit 1;
fi

dir=$1;
if [ ! -d "$dir" ]; then
  echo "Invalid path: ${dir}";
  exit 1;
fi

# Set the lib suffix.
suffix="dylib"
if [[ $(uname -s) == "Linux" ]]; then
  suffix="so"
fi

# the llvm build folder
export LLVM_BUILD_DIR=${dir}/build
# the cmake folder inside the build folder
export LLVM_DIR=$LLVM_BUILD_DIR/lib/cmake
export CLANG=${dir}/build/tools/clang
rm -rf build
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON $LLVM_BUILD_DIR ..
make
