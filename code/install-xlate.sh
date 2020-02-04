#!/bin/bash
cd "$(dirname "$0")"
cd originalcode/xlate

echo "Downloading OpenSSL 1.0.1e"

wget https://ftp.openssl.org/source/old/1.0.1/openssl-1.0.1e.tar.gz
tar -xvzf openssl-1.0.1e.tar.gz
rm openssl-1.0.1e.tar.gz
cd openssl-1.0.1e/

echo "Building OpenSSL"
./config shared no-hw no-asm
make

cd ..
echo "Building XLATE"
make
