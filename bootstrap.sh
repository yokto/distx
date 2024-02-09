#!/bin/bash

set -x
set -e

export HOST_PREFIX="distx.com+2024-"
export ZWOLF="/_zwolf"

git clone "https://github.com/yokto/openlibm.git" openlibm-src
git clone "https://github.com/yokto/llvm-project.git" llvm-src

(cd llvm-src; ../llvm/build-linux.sh)

mkdir -p "/_zwolf/${HOST_PREFIX}xlibc-common"
cp -r xlibc/include "/_zwolf/${HOST_PREFIX}xlibc-common/include"

(cd llvm-src; ../llvm/build-compiler-rt.sh)

(cd xlibc; ZWOLF_INSTALL=/_zwolf make)

(cd openlibm-src; ../openlibm/build.sh)

(cd llvm-src; ../llvm/build-runtime.sh)

(cd llvm-src; ../llvm/build-zwolf.sh)
