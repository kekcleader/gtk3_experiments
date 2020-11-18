PROG=gtk5
CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0`

all: $(PROG)

run: all
	@./$(PROG)

$(PROG): $(PROG).o
	gcc -o $(PROG) $(PROG).o $(LIBS)

$(PROG).o: $(PROG).c
	gcc $(CFLAGS) -c $(PROG).c

.phony: clean run

clean:
	@rm -f *.o $(PROG)
