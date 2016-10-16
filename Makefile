CC         = g++
CFLAGS     = -std=c++1y -O2 -c -Wall -Wextra
LFLAGS     = -std=c++1y -lyaml-cpp
SOURCES    = main.cpp \
             swarm.cpp \
             bee.cpp
OBJECTS    = $(SOURCES:.cpp=.o)
EXECUTABLE = pso

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
