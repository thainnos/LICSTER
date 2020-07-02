#!/bin/bash

echo "++++++++++++++++++++++++++++++++++++++++++++++"
echo "+ Automatic Cert, Key and Makefile Generator +"
echo "+          by Raphael Hausmanninger          +"
echo "++++++++++++++++++++++++++++++++++++++++++++++"


BASE="../projects/Secure_Layer/certs"
MAKE_FILE="../devices/remote_io/software/tls_certs.mk"

declare -a NAMES=("io1" "io2" "plc")
declare -a DEFINES=()

CUR_DATE=`date -u +%Y%m%d%H%M%S`
EXPIRATION=$(expr $CUR_DATE + 00010000000000)


rm -f $MAKE_FILE


echo -n "Generating root CA..."
mkdir -p $BASE
rm -f $BASE/*.crt && rm -f $BASE/*.key

./gen_key       type=rsa rsa_keysize=2048 filename=$BASE/root.key 1>/dev/null
./cert_write    selfsign=1 issuer_key=$BASE/root.key                \
                issuer_name=CN=LICSTER                              \
                not_before=$CUR_DATE not_after=$EXPIRATION          \
                is_ca=1 max_pathlen=0 output_file=$BASE/root.crt 1>/dev/null

echo " done"

echo -n "Generating keys and signing them..."
for NAME in "${NAMES[@]}"; do
    mkdir -p $BASE/${NAME}_certs
    rm -f $BASE/${NAME}_certs/*.crt && rm -f $BASE/${NAME}_certs/*.key

    ./gen_key       type=rsa rsa_keysize=2048 filename=$BASE/${NAME}_certs/$NAME.key 1>/dev/null
    ./cert_write    selfsign=0 issuer_key=$BASE/root.key subject_key=$BASE/${NAME}_certs/$NAME.key  \
                    issuer_name=CN=LICSTER subject_name=CN=$NAME                                    \
                    not_before=$CUR_DATE not_after=$EXPIRATION                                      \
                    output_file=$BASE/${NAME}_certs/$NAME.crt 1>/dev/null
    cat $BASE/${NAME}_certs/${NAME}.crt $BASE/root.crt > $BASE/${NAME}_certs/${NAME}_chain.crt
done
echo " done"

echo -n "Generating Makefile..."
for NAME in "${NAMES[@]}"; do
    if [ "$NAME" = plc ]; then
        continue
    fi;
    for ENDING in "crt" "key"; do
        DEF=`echo "${NAME}_$ENDING" | tr [a-z] [A-Z]`
        DEFINES+=($DEF)
        echo -n "$DEF='$DEF=\"" > .tmp_file
        if [ "$ENDING" = "crt" ]; then
            cat $BASE/${NAME}_certs/${NAME}_chain.$ENDING >> .tmp_file
        else
            cat $BASE/${NAME}_certs/$NAME.$ENDING >> .tmp_file
        fi
        echo "\"'" >> .tmp_file

        (sed ':a;N;$!ba;s/\n/\\r\\n/g' < .tmp_file) >> $MAKE_FILE
    done
    echo "" >> $MAKE_FILE
done

# # add root crt to makefile
# DEF=`echo "ROOT_CRT" | tr [a-z] [A-Z]`
# DEFINES+=($DEF)
# echo -n "$DEF='$DEF=\"" > .tmp_file
# cat $BASE/root.crt >> .tmp_file
# echo "\"'" >> .tmp_file
# (sed ':a;N;$!ba;s/\n/\\r\\n/g' < .tmp_file) >> $MAKE_FILE
# echo "" >> $MAKE_FILE

rm -f .tmp_file

echo -n "C_DEFS +=" >> $MAKE_FILE
for DEFINE in "${DEFINES[@]}";do
    echo -n " -D\$($DEFINE)" >> $MAKE_FILE
done
echo "" >> $MAKE_FILE
echo " done"
