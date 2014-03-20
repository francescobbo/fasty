MAKEFLAGS += --no-print-directory
OUTPUT = app
LIBS = server/libserver.a

all: $(OUTPUT)
	

$(OUTPUT): $(LIBS)
	$(CXX) $(LIBS) -o $@

server/libserver.a:
	cd server && $(MAKE) all
	
clean:
	cd server && $(MAKE) clean
	rm $(OUTPUT)

