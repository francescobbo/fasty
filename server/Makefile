include ../make-common

MAKEFLAGS += --no-print-directory
OUTPUT = libserver.a
OBJS = client_socket.o entry.o http_server.o http2_server.o request_line.o server_socket.o sock_stream.o socket.o

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar -r $@ $(OBJS)
	
clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT)

