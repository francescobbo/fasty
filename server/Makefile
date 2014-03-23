include ../make-common

MAKEFLAGS += --no-print-directory
OUTPUT = libserver.a
OBJS = client_socket.o entry.o server_socket.o sock_stream.o socket.o

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar -r $@ $(OBJS)
	
clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT)

