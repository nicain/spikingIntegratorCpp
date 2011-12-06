CC=g++
INCLUDEFLAGS=-I /Users/nicain/local/lib/boost_1_47_0
RELEASEFLAGS=
SOURCES=spikingIntegrator.cpp Brain.cpp Pool.cpp SpikeList.cpp PoolBG.cpp PoolBGFile.cpp PoolRec.cpp PoolRecEx.cpp PoolRecInh.cpp Monitor.cpp MonitorNeuron.cpp MonitorNeuronFile.cpp MonitorBrain.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SI

all: $(SOURCES) $(EXECUTABLE)

release: RELEASEFLAGS= -O3 -funit-at-a-time -DNDEBUG
release: EXECUTABLE=SI_release
release: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(INCLUDEFLAGS) $(RELEASEFLAGS) $(OBJECTS) -o $(EXECUTABLE)

.cpp.o:
	$(CC) $(INCLUDEFLAGS) $(RELEASEFLAGS) $< -c

tar:
	tar -cvzf $(EXECUTABLE).tar.gz $(SOURCES) Makefile

clean:
	rm *.o SI SI_release $(EXECUTABLE).tar.gz