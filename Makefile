#          Copyright Emil Fresk 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE.md or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

all:
	make all -C ./examples/blinky
	make all -C ./examples/unique

clean:
	make clean -C ./examples/blinky
	make clean -C ./examples/unique
