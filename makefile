EXE := sudoku_solver
CC := gcc
CFLAGS := -O0 -Wall -Wextra
SRC := .c

OBJS := $(patsubst %$(SRC), %.o, $(wildcard *$(SRC)))
DEPS := $(OBJS:.o=.d)

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

-include $(DEPS)
%.o: %$(SRC)
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

clean:
	rm $(EXE) $(OBJS) $(DEPS)