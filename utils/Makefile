include ../make-common

MAKEFLAGS += --no-print-directory
OUTPUT = libutils.a
OBJS = blob.o string.o

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar -r $@ $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT)

