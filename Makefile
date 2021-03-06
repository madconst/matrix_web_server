CC      = g++
CFLAGS      = -g -c -Wall -pthread --std=c++0x
LDFLAGS     = -pthread --std=c++0x
SOURCES     = main.cpp web_server.cpp socket_stream.cpp tcp_server.cpp request_processor.cpp
INCLUDES    = -I.
OBJECTS     = $(SOURCES:.cpp=.o)
TARGET      = web_server

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)

test:
	$(CC) -O0 $(LDFLAGS) test_matrix.cpp -o test_matrix
