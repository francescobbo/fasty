include ../make-common

MAKEFLAGS += --no-print-directory
OUTPUT = libframework.a
OBJS = request.o

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar -r $@ $(OBJS)
	
clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT)

