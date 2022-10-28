T1 = sort

CC = g++
CFLAGS = -c -Wall -g -O3 -std=c++1z
LDFLAGS = -lm -lpthread -g

OBJ1 = sort.o
OBJ5 = lib.o
#
# Rules for make
#
all: $(T1) 


$(T1): $(OBJ1) $(OBJ5)
	$(CC) -o $@ $^ $(LDFLAGS)

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o *.exe *.stackdump $(T1)
