LDFLAGS = -g
CFLAGS  = -g -Wall -Wextra -Werror
DST     = gen-static-sbox
OBJS    = gen-static-sbox.o

all: $(DST)

$(DST): $(OBJS)
	gcc $(LDFLAGS) $(OBJS) -o $(DST)

%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o

clean:
	rm -f *.o $(DST)


sboxes:
	cat BJNR000010949.epub | openssl aes-256-cbc -kfile BJNR000010949.pass | ./$(DST)
