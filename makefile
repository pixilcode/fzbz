CC = gcc
CFLAGS = -Wall -Werror

# default target
all: c

# c build
c: build-dir
	$(CC) $(CFLAGS) c/main.c -o build/fzbz-c

run-c: c
	./build/fzbz-c

# ensure the build directory exists
build-dir:
	mkdir -p build

# clean the build files
clean:
	rm -rf build
