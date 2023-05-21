CC := clang
SRCS := automata.c
OBJS := $(SRCS:.c=.o)
EXEC := automata
HDRS := include/*.h
CFLAGS := -I/Library/Frameworks/SDL2.framework/Headers 
LDFLAGS := -F/Library/Frameworks -framework SDL2

.PHONY: all clean rebuild

rebuild: clean all

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJS): $(@:.o=.c) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(@:.o=.c) -c

clean:
	rm -f $(EXEC) $(OBJS) *.ppm
