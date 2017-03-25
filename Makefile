
all:
	make all -C ./examples/blinky
	make all -C ./examples/unique

clean:
	make clean -C ./examples/blinky
	make clean -C ./examples/unique
