BINARY=gitcrypt
SRC=src/main.cpp

all: $(BINARY)

$(BINARY): $(SRC)
	g++ -o $(BINARY) $(SRC) -Wall -I./src -lz -lcrypto -lboost_regex

remake: clean all

clean:
	rm $(BINARY)

install:
	cp $(BINARY) /usr/local/bin
