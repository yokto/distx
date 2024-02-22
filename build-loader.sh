#!/bin/bash

set -x
set -e

export HOST_PREFIX="distx.org_2024-"

type x86_64-w64-mingw32-gcc

cd xload
make

cd _zwolf
tar cf "${HOST_PREFIX}xload-x86_64.tar.xz" "${HOST_PREFIX}xload-x86_64"
tar cf "${HOST_PREFIX}xload-aarch64.tar.xz" "${HOST_PREFIX}xload-aarch64"

set +x
s3cmd --access_key AKIA3UPFWFC4ANF5H3TC --secret_key "${AWS_KEY}" put "${HOST_PREFIX}xload-x86_64.tar.xz" s3://distx
s3cmd --access_key AKIA3UPFWFC4ANF5H3TC --secret_key "${AWS_KEY}" put "${HOST_PREFIX}xload-aarch64.tar.xz" s3://distx
set -x
