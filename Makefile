CC         = g++
CFLAGS     = -std=c++14 -O2 -c -Wall -Wextra
LFLAGS     = -std=c++14 -lyaml-cpp
SOURCES    = main.cpp \
             swarm.cpp \
             bee.cpp
OBJECTS    = $(SOURCES:.cpp=.o)
EXECUTABLE = bso

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE) $(EXECUTABLE).log
