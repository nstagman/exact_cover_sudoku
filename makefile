EXE := s_solve
CC := gcc
CFLAGS := -Wall -Wextra
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