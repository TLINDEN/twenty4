#!/bin/sh

an=./analyze
cr=./stream

mkdir -p t

jot () { 
    r=$1;
    if test -z "$r"; then
        echo "jot <rounds>";
    else
        perl -e "foreach(0..$r){print \"\$_\n\";}";
    fi
}

./prepare-analyze

for L in `jot 255`; do
    XL=`printf "%02x" $L`
    clear="t/$XL.clear"
    for K in `jot 255`; do
	XK=`printf "%02x" $K`
	cipher="t/$XLx$XK.cipher"
	cat $clear | $cr 0x$K e > $cipher
	echo -n "$XL w/ $XK: "
	$an $cipher 1
	rm -f $cipher
    done
done


