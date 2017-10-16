#!/bin/bash
#read line by line
i=1
for line in $(cat /etc/passwd)
do
    i=$((i+1))
    echo $i":"$line | sed  's/^\([^:]*\):\([^:]*\):\([^:]*\):\([0-9]*\):\([0-9]*\)::\([^:]*\).*/\1) Username: \2 ,Password: encrypted, UID: \4, GID: \5, Home: \6/'
done

