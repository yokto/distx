#!/bin/bash

set -x
set -e

export HOST_PREFIX="distx.org+2024-"
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

mkdir -p "${ZWOLF}/old"
mv ${ZWOLF}/${HOST_PREFIX}llvm-{common,dev-*,doc-*,x86_64} ${ZWOLF}/old
mv ${ZWOLF}/tmp/_zwolf/* ${ZWOLF}

(cd ${ZWOLF};
for x in ${HOST_PREFIX}*
do
	tar cf "$x.tar.xz" "$x"
done
)

FTP_SERVER="ftpupload.net"
FTP_USERNAME="if0_35948865"

# Remote directory to upload to
REMOTE_DIR="/htdocs"

(cd ${ZWOLF};
for LOCAL_FILE in ${HOST_PREFIX}*tar.xz
do
	
ftp -n $FTP_SERVER <<END_SCRIPT
quote USER $FTP_USERNAME
quote PASS $FTP_PASSWORD
cd $REMOTE_DIR
put $LOCAL_FILE
quit
END_SCRIPT

done
)
