include ../make-common

MAKEFLAGS += --no-print-directory
OUTPUT = libframework.a
OBJS = base64.o request.o secure_random.o

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar -r $@ $(OBJS)
	
clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT)

