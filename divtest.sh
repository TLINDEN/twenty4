#!/bin/sh

in=111111111111

echo "- same clear text slightly different key"
for K in 1 2 3 4 5 6 7 8; do
    k="0000000000000000000$K"
    echo -n "IN: $in, KEY: $k, NONCE: $K, OUT: "
    echo -n "$in" | ./twenty4 $k $K | hexdump | head -1 | sed 's/^0000000 //'
done

echo

k="00000000000000000001"
echo "- same key, slightly different clear text"
for N in 1 2 3 4 5 6 7 8; do
    in="11111111111$N"
    echo -n "IN: $in, KEY: $k, NONCE: $N, OUT: "
    echo -n "$in" | ./twenty4 $k $N | hexdump | head -1 | sed 's/^0000000 //'
done
