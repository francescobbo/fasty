include ../make-common

MAKEFLAGS += --no-print-directory
OUTPUT = libserver.a
OBJS = entry.o socket.o client_socket.o server_socket.o

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar -r $@ $(OBJS)
	
clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT)

