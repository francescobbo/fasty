include ../make-common

MAKEFLAGS += --no-print-directory
OUTPUT = libutils.a
OBJS =

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar -r $@ $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT)

