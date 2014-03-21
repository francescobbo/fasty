MAKEFLAGS += --no-print-directory -s
OUTPUT = app
LIBS = server/libserver.a utils/libutils.a

all: $(OUTPUT)
	

$(OUTPUT): libs $(LIBS)
	$(CXX) $(LIBS) -o $@

libs:
	cd server && $(MAKE) all
	cd utils && $(MAKE) all
	
clean:
	cd server && $(MAKE) clean
	cd utils && $(MAKE) clean
	rm $(OUTPUT)

