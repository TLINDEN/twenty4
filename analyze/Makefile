LDFLAGS = -g
CFLAGS  = -g -Wall -Wextra -Werror
DST     = analyze
OBJS    = analyze.o

all: $(DST)

$(DST): $(OBJS)
	gcc $(LDFLAGS) $(OBJS) -lm -lz -o $(DST)

%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o

clean:
	rm -f *.o $(DST)

