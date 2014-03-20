include ../make-common

MAKEFLAGS += --no-print-directory
OUTPUT = libserver.a
OBJS = entry.o

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar -r $@ $(OBJS)

server/libserver.a:
	cd server && $(MAKE) all
