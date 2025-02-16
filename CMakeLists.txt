cmake_minimum_required(VERSION 3.10)
project(MyPPCallbacksPlugin LANGUAGES C CXX)

# Clang/LLVM がインストールされているパスを設定
# 例: -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm
# などの形で cmake に教えてあげる
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM at: ${LLVM_DIR}")
message(STATUS "Found Clang at: ${Clang_DIR}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# LLVM/Clang の設定を CMake に反映
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./include)
add_subdirectory(macker)
