GCC := gcc

GCC_ARGS += -O3
GCC_ARGS += -Wall
GCC_ARGS += -D__SPFP_MAIN
GCC_ARGS += -I./inc

SIMPLE_OBJECTS += src/test/test.o
SIMPLE_OBJECTS += src/spfp.o

LINUX_OBJECTS += src/helpers/linux.o
LINUX_OBJECTS += src/test/linux.o
LINUX_OBJECTS += src/spfp.o

%.o: %.c
	$(GCC) $(GCC_ARGS) -c $< -o $@

simple: $(SIMPLE_OBJECTS)
	$(GCC) $(GCC_ARGS) $(SIMPLE_OBJECTS)

linux: $(LINUX_OBJECTS)
	$(GCC) $(GCC_ARGS) $(LINUX_OBJECTS)

clean:
	rm -rf $(TEST_OBJECTS)