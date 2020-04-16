#!/bin/bash
mkdir plc_certs && pushd plc_certs
rm *.crt && rn *.key
openssl req -new -newkey rsa:2048 -days 365 -nodes -x509 -keyout plc.key -out plc.crt
popd
mkdir io_certs && pushd io_certs
rm *.crt && rn *.key
openssl req -new -newkey rsa:2048 -days 365 -nodes -x509 -keyout io1.key -out io1.crt
popd
