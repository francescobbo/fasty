MAKEFLAGS += --no-print-directory
OUTPUT = app
LIBS = server/libserver.a utils/libutils.a

all: $(OUTPUT)
	

$(OUTPUT): $(LIBS)
	$(CXX) $(LIBS) -o $@

server/libserver.a:
	cd server && $(MAKE) all

utils/libutils.a:
	cd utils && $(MAKE) all
	
clean:
	cd server && $(MAKE) clean
	rm $(OUTPUT)

