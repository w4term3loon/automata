CC := clang
SRCS := automata.c
OBJS := $(SRCS:.c=.o)
EXEC := automata
HDRS := include/*.h
CFLAGS := $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs)

.PHONY: all clean rebuild

rebuild: clean all

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJS): $(@:.o=.c) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(@:.o=.c) -c

clean:
	rm -f $(EXEC) $(OBJS) *.ppm
