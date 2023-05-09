CC := clang

SRCS := life.c
OBJS := $(SRCS:.c=.o)
EXEC := automata
HDRS := include/*.h

rebuild: clean | all

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS)

$(OBJS): $(@:.o=.c) $(HDRS) Makefile
	$(CC) -o $@ $(@:.o=.c) -c

clean:
	rm -f $(EXEC) $(OBJS) *.ppm
