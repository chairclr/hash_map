clean:
	rm -rf build

release:
	mkdir build
	clang main.c hash_map.c -I. -o ./build/hash -O2

all:
	$(MAKE) clean
	$(MAKE) release

.DEFAULT_GOAL=all
.PHONY=clean

