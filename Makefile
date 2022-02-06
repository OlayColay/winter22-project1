CC=g++
CFLAGS=-I.
DEPS=
OBJ=server.o
COBJ=client.o
USERID=005376979

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: server
server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
# client: $(COBJ)
# 	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o server client *.tar.gz

dist: tarball
tarball: clean
	tar -cvzf /tmp/$(USERID).tar.gz --exclude=./.vagrant . && mv /tmp/$(USERID).tar.gz .