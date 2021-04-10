GCC := gcc

GCC_ARGS += -O3
GCC_ARGS += -Wall
GCC_ARGS += -D__SPFP_MAIN

SOURCES += main.c
SOURCES += spfp.c

OBJECTS := $(SOURCES:.c=.o)

%.o: %.c
	$(GCC) $(GCC_ARGS) -c $< -o $@

all: $(OBJECTS)
	$(GCC) $(GCC_ARGS) $(OBJECTS)