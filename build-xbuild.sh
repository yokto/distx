#!/bin/bash

set -x
set -e

export HOST_PREFIX="distx.org_2024-"

cd xbuild
../distx.py i
make

cd _distx
tar cf "${HOST_PREFIX}xbuild-x86_64.tar.xz" "${HOST_PREFIX}xbuild-x86_64"
tar cf "${HOST_PREFIX}xbuild-aarch64.tar.xz" "${HOST_PREFIX}xbuild-aarch64"

set +x
s3cmd --access_key AKIA3UPFWFC4ANF5H3TC --secret_key "${AWS_KEY}" put "${HOST_PREFIX}xbuild-x86_64.tar.xz" s3://distx
s3cmd --access_key AKIA3UPFWFC4ANF5H3TC --secret_key "${AWS_KEY}" put "${HOST_PREFIX}xbuild-aarch64.tar.xz" s3://distx
set -x
