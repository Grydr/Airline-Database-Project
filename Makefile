CC = gcc
CFLAGS = -Wall -Wextra -O0 -g3 -I./lib 

SRCDIR = src
LIBDIR = lib
DATADIR = data
BUILDDIR = build

SRCS = $(SRCDIR)/airline.c $(LIBDIR)/flightlib.c
OBJS = $(SRCS:.c=.o)

TARGET = airline

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile source files
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBDIR)/%.o: $(LIBDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(SRCDIR)/*.o $(LIBDIR)/*.o $(TARGET)

# Phony targets
.PHONY: default clean
