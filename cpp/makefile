EXE := ssc
CC := clang++
CFLAGS := -O3 -Wall -Wextra
LFLAGS := -lm
SRC := .cpp

OBJS := $(patsubst %$(SRC), %.o, $(wildcard *$(SRC)))
DEPS := $(OBJS:.o=.d)

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS) $(LFLAGS)

-include $(DEPS)
%.o: %$(SRC)
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

clean:
	rm $(EXE) $(OBJS) $(DEPS)



# Run Python
py:
	python3 ../python/solver.py