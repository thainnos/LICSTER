#!/bin/bash

BASE="../devices/plc/software/Secure_Layer/certs"
MAKE_FILE="../devices/remote_io/software/tls_certs.mk"

declare -a NAMES=("io1" "io2" "plc")
declare -a DEFINES=()

CUR_DATE=`date -u +%Y%m%d%H%M%S`
EXPIRATION=$(expr $CUR_DATE + 00010000000000)


rm -f $MAKE_FILE

echo -n "Generating certificates and keys (this might take a while)..."
for NAME in "${NAMES[@]}"; do
    mkdir -p $BASE/${NAME}_certs
    rm -f $BASE/${NAME}_certs/*.crt && rm -f $BASE/${NAME}_certs/*.key

    ./gen_key type=rsa rsa_keysize=2048 filename=$BASE/${NAME}_certs/$NAME.key 1>/dev/null
    ./cert_write selfsign=1 issuer_key=$BASE/${NAME}_certs/$NAME.key                    \
                         issuer_name=CN=$NAME,O=LICSTER        \
                         not_before=$CUR_DATE not_after=$EXPIRATION   \
                         is_ca=1 max_pathlen=0 output_file=$BASE/${NAME}_certs/$NAME.crt 1>/dev/null
done
echo " done"

echo -n "Generating Makefile..."
for NAME in "${NAMES[@]}"; do
    for ENDING in "crt" "key"; do
        DEF=`echo "${NAME}_$ENDING" | tr [a-z] [A-Z]`
        DEFINES+=($DEF)
        echo -n "$DEF='$DEF=\"" > .tmp_file
        cat $BASE/${NAME}_certs/$NAME.$ENDING >> .tmp_file
        echo "\"'" >> .tmp_file

        (sed ':a;N;$!ba;s/\n/\\r\\n/g' < .tmp_file) >> $MAKE_FILE
    done
    echo "" >> $MAKE_FILE
done
rm -f .tmp_file

echo -n "C_DEFS +=" >> $MAKE_FILE
for DEFINE in "${DEFINES[@]}";do
    echo -n " -D\$($DEFINE)" >> $MAKE_FILE
done
echo "" >> $MAKE_FILE
echo " done"
