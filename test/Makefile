INCLUDE = -I../../brigand -I../ -I./inc
DEFS = -DBRIGAND_NO_BOOST_SUPPORT -DNDEBUG
FLAGS = -std=c++14 -Wall -Wextra -Wfatal-errors -O1 -ggdb
FLAGS2 = -fno-rtti -fno-exceptions -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -fomit-frame-pointer -falign-functions=16 -mno-thumb-interwork
ARM_FLAGS = -c $(FLAGS) $(FLAGS2)
LDFLAGS = $(FLAGS2) -T link.ld -Wl,-Map=build/output.map --specs=nosys.specs -nostdlib

all:
		@mkdir -p build
		arm-none-eabi-g++ $(INCLUDE) $(DEFS) $(ARM_FLAGS) test.cpp -o build/test.o
		arm-none-eabi-g++ $(INCLUDE) $(DEFS) $(ARM_FLAGS) ../rtfm/rtfm_vector_table.cpp -o build/vector.o
		arm-none-eabi-g++ $(LDFLAGS) build/test.o build/vector.o -o build/test.elf
		arm-none-eabi-objdump -D build/test.elf > build/test.dmp

clean:
		rm -rf build

test:
		@mkdir -p build
		clang++ $(INCLUDE) $(DEFS) $(FLAGS) test.cpp -o build/test
