LDFLAGS = -g -O3
CFLAGS  = -g -O3 -Wall -Wextra -Werror
DST     = twenty4
OBJS    = twenty4.o
K       = 123456trewqasdfgbvcx

all: $(DST)
	cd analyze && make
	cd sbox && make

$(DST): $(OBJS)
	gcc $(LDFLAGS) $(OBJS) -o $(DST)
	@rm -f x

%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o

clean:
	rm -f *.o $(DST) *.core zero x*
	cd analyze && make clean
	cd sbox && make clean

test: $(DST) zero x decrypttest antest enttest
	@echo "run 'make dietest' for dieharder tests, use screen"



zero:
	dd if=/dev/zero of=zero bs=1024 count=10000

x:
	cat zero | ./twenty4 00000000000000000001 1 > x

decrypttest: $(DST)
	sha256 LICENSE | cut -f 4 -d ' ' > .xsum
	cat LICENSE   | ./$(DST) $(K) X  > .xenc
	cat .xenc     | ./$(DST) $(K) X  > .xdec
	sha256 .xdec  | cut -f 4 -d ' '  > .xnsum
	if test "x`cat .xsum`" = "x`cat .xnsum`"; then echo OK; else echo FAIL; fi
	rm -f .x*
	./divtest.sh
	@echo

# my own
antest: $(DST) zero x
	analyze/analyze x
	@echo

# ent from: http://www.fourmilab.ch/random/
enttest: $(DST) x
	ent x
	@echo

# dieharder from: https://www.phy.duke.edu/~rgb/General/dieharder.php
dietest: $(DST)
	dd if=/dev/zero of=/dev/stdout | ./twenty4 00000000000000000001 1 | dieharder -g 200 -a
