#CC          = arm-linux-gcc
#MACH        = gcc_arm

MACH_HEADER ?= include/mach/$(MACH).h
RM          ?= rm -f
CFLAGS 		?= -Iinclude -DCOROUTINE_MACH=\"$(MACH_HEADER)\"

.PHONY : clean all

all : ex1

coroutine.o : coroutine.c include/coroutine.h $(MACH_HEADER)
	$(CC) $(CFLAGS) -c coroutine.c

ex1.o : example/ex1.c include/coroutine.h
	$(CC) $(CFLAGS) -c example/ex1.c

ex1 : ex1.o coroutine.o
	$(CC) ex1.o coroutine.o $(LDFLAGS) -o ex1

clean:
	$(RM) coroutine.o
	$(RM) ex1 ex1.o

