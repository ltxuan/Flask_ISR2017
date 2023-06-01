# If your application is in a file named myapp.cpp or myapp.c
# this is the line you will need to build the binary.
all: app

app: main.c
	$(CC) -o $@ read_config.c uart.c $< `pkg-config --cflags --libs libpjproject` `pkg-config --cflags --libs libtiff-4` -lspandsp -lspandsp-sim -lsndfile -ltiff

clean:
	rm -f app.o app
