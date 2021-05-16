CC = gcc
LD = gcc

RM = rm -rf

DEBUG_FLAGS = #-g
WARNING_FLAGS = #-Wall

CFLAGS   = -MD -pthread $(DEBUG_FLAGS) $(WARNING_FLAGS) -D_GNU_SOURCE -std=c99
LDFLAGS  = $(CFLAGS) -lm

OBJS = main.o calc.o CPUTopology.o
EXEC_FILE = main.out

# Main ----------------------------------------------------

# Required to build the executable file
ALL_EXEC_OBJS = $(OBJS)

all: $(EXEC_FILE)

$(EXEC_FILE): $(OBJS)
	$(LD) -o $@ $(ALL_EXEC_OBJS) $(LDFLAGS)


# Clean ----------------------------------------------------

clean_temp:
	$(RM) *.o *.d *.gch

clean: clean_temp
	$(RM) $(EXEC_FILE)

-include $(ALL_EXEC_OBJS:.o=.d)

.PHONY: all hpc_lib test_hpc_lib test clean