CC=g++
INCLUDEFLAGS=-I /Users/nicain/local/lib/boost_1_47_0
RELEASEFLAGS=
SOURCES=spikingIntegrator.cpp SI_functions.cpp BGPool.cpp SI_settings.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SI

all: $(SOURCES) $(EXECUTABLE)

release: RELEASEFLAGS=-DNDEBUG
release: EXECUTABLE=SI_release
release: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(INCLUDEFLAGS) $(RELEASEFLAGS) $(OBJECTS) -o $(EXECUTABLE)

.cpp.o:
	$(CC) $(INCLUDEFLAGS) $(RELEASEFLAGS) $< -c
clean:
	rm *.o SI SI_release