#!/bin/bash

set -x
set -e

export HOST_PREFIX="distx.org_2024-"
export DISTX_PREFIX="distx.org_2024-"
export ZWOLF="/_zwolf"

set +x
#s3cmd --access_key AKIA3UPFWFC4ANF5H3TC --secret_key "${AWS_KEY}" put README.md s3://distx
s3cmd --access_key b5ed2484c5ddd13d820875d1c2384ab6 --secret_key "${AWS_KEY}" --host=4b8146e0834dbdc46e201fbbb29fa315.r2.cloudflarestorage.com --host-bucket '%(bucket)s.4b8146e0834dbdc46e201fbbb29fa315.r2.cloudflarestorage.com' put "README.md" s3://dist
set -x

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
(cd llvm-src; ARCH=aarch64 ../llvm/build-zwolf.sh)

mkdir -p "${ZWOLF}/old"
mv ${ZWOLF}/${HOST_PREFIX}llvm-{common,dev-*,doc-*,x86_64} ${ZWOLF}/old
mv ${ZWOLF}/tmp/_zwolf/* ${ZWOLF}

(cd ${ZWOLF};
for x in ${HOST_PREFIX}*
do
	tar cf "$x.tar.xz" "$x"
done
)

(cd ${ZWOLF};
for LOCAL_FILE in ${HOST_PREFIX}*tar.xz
do
	echo $LOCAL_FILE
	set +x
	s3cmd --access_key b5ed2484c5ddd13d820875d1c2384ab6 --secret_key "${AWS_KEY}" --host=4b8146e0834dbdc46e201fbbb29fa315.r2.cloudflarestorage.com --host-bucket '%(bucket)s.4b8146e0834dbdc46e201fbbb29fa315.r2.cloudflarestorage.com' put "${LOCAL_FILE}" s3://dist
	#s3cmd --access_key AKIA3UPFWFC4ANF5H3TC --secret_key "${AWS_KEY}" put "${LOCAL_FILE}" s3://distx
	set -x
done
)
