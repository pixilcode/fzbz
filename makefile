CC = gcc
CFLAGS = -Wall -Werror

# default target
all: c

# c build
c: build-dir
	$(CC) $(CFLAGS) c/main.c -o build/fzbz-c

run-c: c
	./build/fzbz-c

# ocaml build
ocaml: build-dir
	ocamlbuild -pkg unix ocaml/main.native -build-dir build
	mv build/ocaml/main.native build/fzbz-ocaml
	rm -r build/ocaml build/ocamlc.where

run-ocaml: ocaml
	./build/fzbz-ocaml

# ensure the build directory exists
build-dir:
	mkdir -p build

# clean the build files
clean:
	rm -rf build
