INCLUDE = -I../brigand -I../eigen -I./
DEFS = -DBRIGAND_NO_BOOST_SUPPORT -DNDEBUG
FLAGS = -std=c++14 -Wall -Wextra -O2
ARM_FLAGS = $(FLAGS) -c -fno-rtti -fno-exceptions -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb -fomit-frame-pointer -falign-functions=16

all:
		@mkdir -p build
		arm-none-eabi-g++ $(INCLUDE) $(DEFS) $(ARM_FLAGS) test.cpp -o build/test.o
		arm-none-eabi-objdump -D build/test.o > build/test.dmp

clean:
		rm -rf build

test:
		@mkdir -p build
		clang++ $(INCLUDE) $(DEFS) $(FLAGS) test.cpp -o build/test
