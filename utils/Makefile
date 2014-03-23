include ../make-common

MAKEFLAGS += --no-print-directory
OUTPUT = libutils.a
OBJS = blob.o dir.o http_status_codes.o file.o string.o uri.o

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar -r $@ $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT)

