LDFLAGS = -g
CFLAGS  = -g -Wall -Wextra -Werror
DST     = twenty4
OBJS    = twenty4.o

all: $(DST)
	cd analyze && make
	cd sbox && make

$(DST): $(OBJS)
	gcc $(LDFLAGS) $(OBJS) -o $(DST)

%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o

clean:
	rm -f *.o $(DST)
	cd analyze && make clean
	cd sbox && make clean
