#!/bin/bash
cd "$(dirname "$0")"
cd originalcode/xlate

echo "Downloading OpenSSL 1.1.1b"
wget https://www.openssl.org/source/old/1.1.1/openssl-1.1.1b.tar.gz
tar -xf openssl-1.1.1b.tar.gz
rm openssl-1.1.1b.tar.gz
cd openssl-1.1.1b

echo "Building OpenSSL"
./config shared no-hw no-asm
make

cd ..
echo "Building XLATE"
make
