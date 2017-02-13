INCLUDE = -I../../brigand -I../ -I./inc
DEFS = -DBRIGAND_NO_BOOST_SUPPORT -DNDEBUG
FLAGS = -std=c++14 -Wall -Wextra -Wfatal-errors -ggdb -O1
COMMON = -fno-rtti -fno-exceptions -mcpu=cortex-m4 -mfloat-abi=hard
COMMON += -mfpu=fpv4-sp-d16 -mthumb -fomit-frame-pointer -falign-functions=16
COMMON += -mno-thumb-interwork -fno-threadsafe-statics -nostartfiles
COMMON += -fsigned-char -fno-move-loop-invariants -ffunction-sections
COMMON += -fdata-sections -Xlinker --gc-sections
ARM_FLAGS = -c $(FLAGS) $(COMMON)
LDFLAGS = $(COMMON) -T link.ld -Wl,-Map=build/output.map --specs=nano.specs

OCD_TARGET = stm32f4x
OCD_INTERFFACE = stlink-v2-1.cfg
OPENOCD = openocd

GDB_FLAGS = -tui -ex "target remote | $(OPENOCD) -c \"gdb_port pipe; log_output openocd.log\" -f interface/$(OCD_INTERFFACE) -f target/$(OCD_TARGET).cfg" -ex "load" -ex "monitor reset halt" -ex "set print pretty on" -ex "focus cmd" -ex "winheight SRC -8"

all:
		@mkdir -p build
		arm-none-eabi-g++ $(INCLUDE) $(DEFS) $(ARM_FLAGS) test.cpp -o build/test.o
		arm-none-eabi-g++ $(INCLUDE) $(DEFS) $(ARM_FLAGS) init.cpp -o build/init.o
		arm-none-eabi-g++ $(INCLUDE) $(DEFS) $(ARM_FLAGS) ../rtfm/rtfm_vector_table.cpp -o build/vector.o
		arm-none-eabi-g++ $(LDFLAGS) build/test.o build/vector.o build/init.o -o build/test.elf
		arm-none-eabi-objdump -D build/test.elf > build/test.dmp
		@echo " "
		@arm-none-eabi-size --format=berkeley build/test.elf


gdb:
	@echo "Starting GDB..."
	@arm-none-eabi-gdb build/test.elf $(GDB_FLAGS)


clean:
		rm -rf build

pc_test:
		@mkdir -p build
		clang++ $(INCLUDE) $(DEFS) -DPC_DEBUG $(FLAGS) test.cpp -o build/test
