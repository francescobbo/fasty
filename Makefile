MAKEFLAGS += --no-print-directory -s
OUTPUT = app
LIBS = server/libserver.a utils/libutils.a framework/libframework.a

all: $(OUTPUT)
	

$(OUTPUT): libs $(LIBS)
	$(CXX) $(LIBS) -L/usr/local/ssl/lib -lpthread -lssl -lcrypto -ldl -o $@

libs:
	cd framework && $(MAKE) all
	cd server && $(MAKE) all
	cd utils && $(MAKE) all
	
clean:
	cd framework && $(MAKE) clean
	cd server && $(MAKE) clean
	cd utils && $(MAKE) clean
	rm -f $(OUTPUT)

