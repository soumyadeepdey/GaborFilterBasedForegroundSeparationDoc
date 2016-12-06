#CC=g++-4.4
CFLAGS=-finline-functions  -I /usr/local/include/opencv -msse -I tclap -O3 
LDFLAGS=-lhighgui -lcxcore -lcv
SOURCES=base64.cpp encoding.cpp main.cpp maths.cpp model.cpp  \
	pls.cpp  Storage.cpp


OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(LIBS) $(SOURCES)  $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -c

clean:
	rm -f *.o $(EXECUTABLE)
