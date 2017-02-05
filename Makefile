INCLUDE = -I../brigand -I../eigen -I../CMSIS_5 -I./
DEFS = -DBRIGAND_NO_BOOST_SUPPORT
FLAGS = -std=c++14 -Wall -Wextra -O2
ARM_FLAGS = $(FLAGS) -c -fno-rtti -fno-exceptions -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb -fomit-frame-pointer -falign-functions=16

all:
		arm-none-eabi-g++ $(INCLUDE) $(DEFS) $(ARM_FLAGS) test.cpp
		arm-none-eabi-objdump -D test.o > test.dmp

