#!/bin/bash

# 定义路径变量
PROJECT_DIR="$(pwd)"
BUILD_DIR="${PROJECT_DIR}/build"
INSTALL_DIR="${PROJECT_DIR}/install"

BUILD_TYPE="Release"

# 创建构建目录
if [ ! -d "$BUILD_DIR" ]; then
  echo "Creating build directory: $BUILD_DIR"
  mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR" || exit

# 如果传入 "clean" 参数，清理构建目录并退出
if [ "$1" == "clean" ]; then
  echo "Cleaning build directory..."
  rm -rf "$BUILD_DIR"/*
  exit 0
fi

# 如果传入 "install" 参数，跳过构建，只执行安装
if [ "$1" == "install" ]; then
  echo "Configuring CMake for installation..."
  cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
        "$PROJECT_DIR"

  if [ $? -ne 0 ]; then
    echo "Configuring failed"
    exit 1
  fi

  echo "Start installing project"
  make install

  if [ $? -ne 0 ]; then
    echo "Installing failed"
    exit 1
  fi

  echo "Installing successful"
  exit 0
fi

# 默认配置和构建项目
echo "Configuring CMake"
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      "$PROJECT_DIR"

if [ $? -ne 0 ]; then
  echo "Configuring CMake failed!"
  exit 1
fi

echo "Start building project..."
make -j$(nproc)

if [ $? -ne 0 ]; then
  echo "Building failed!"
  exit 1
fi

echo "Build completed successfully!"

# 如果传入 "install" 参数，执行安装
if [ "$2" == "install" ]; then
  echo "Start installing project..."
  make install

  if [ $? -ne 0 ]; then
    echo "Installing failed!"
    exit 1
  fi

  echo "Installing successful!"
else
  echo "Build completed successfully, but not installing."
fi
