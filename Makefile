CC=gcc
CFLAGS= -Wall -Wunused -Werror
CPPFLAGS= -DDEBUG
LDFLAGS=
BIN=ra_check
SOURCES=ra_check.c
OBJECTS=$(SOURCES:.c=.o)

all=$(BIN)

$(BIN):$(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@
	@rm -rf ./*.o ./*.gch

#ra_check.o: ra_check.c ra_check.h
#	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

.PHONY: clean 
clean:
	rm -f ./*.gch
	rm -f ./*.o
	rm -f ./$(BIN)
