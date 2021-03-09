CC = gcc
LD = gcc
RM = rm -rf

CFLAGS = -g -Wall
LDFLAGS = $(CFLAGS)

CFLAGS_COV  += -fprofile-arcs -ftest-coverage
LDFLAGS_COV += -lgcov

CFLAGS += $(CFLAGS_COV)
LDFLAGS += $(LDFLAGS_COV)

TEST_SRC = RBTree/tests/utests.c
LIB_SRC  = RBTree/src/RBTree.c

TEST_OBJS = $(TEST_SRC:.c=.o)
LIB_OBJS = $(LIB_SRC:.c=.o)

EXEC = tree.out
TEST_EXEC = test.out

LCOV_OUT_FOLDER = out
BROWSER = chromium

all: $(EXEC)

$(EXEC): $(LIB_OBJS)
	$(LD) -o $@ main.c $^ $(LDFLAGS)

test: test_build
	./$(TEST_EXEC)

test_build: build_test_lib $(TEST_OBJS)
	$(LD) -o $(TEST_EXEC) main.c $(LIB_OBJS) $(TEST_OBJS) $(LDFLAGS) -DRUN_TESTS

build_test_lib:
	$(CC) $(LIB_SRC) -o $(LIB_OBJS) -c -DRUN_TESTS $(LDFLAGS)

view_coverage: test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info -o $(LCOV_OUT_FOLDER)
	$(BROWSER) $(LCOV_OUT_FOLDER)/index.html

valgrind: test_build
	valgrind ./$(TEST_EXEC)


clean:
	$(RM) 	$(LIB_OBJS) $(LIB_OBJS:.o=.gcno) $(LIB_OBJS:.o=.gcda) 	 \
			$(TEST_OBJS) $(TEST_OBJS:.o=.gcno) $(TEST_OBJS:.o=.gcda) \
			$(LCOV_OUT_FOLDER) coverage.info						 \
			$(EXEC) $(TEST_EXEC) *.o *.gcno *.gcda

-include $(TOBJ:.o=.d) $(LOBJ:.o=.d)

.PHONY: clean build test view_coverage
