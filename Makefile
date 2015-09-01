LDFLAGS = -g
CFLAGS  = -g -Wall -Wextra -Werror
DST     = twenty4
OBJS    = twenty4.o

all: $(DST)

$(DST): $(OBJS)
	gcc $(LDFLAGS) $(OBJS) -o $(DST)

%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o

clean:
	rm -f *.o $(DST)

