CFLAGS+=-pthread
LDFLAGS+=-pthread


all:server  clean

server:server.o thr_list.o medialib.o thr_channel.o
	$(CC) -o  $@   $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o 