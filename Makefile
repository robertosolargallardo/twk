CXX=mpicxx
CXXFLAGS=-lbsponmpi -O3 -g -ggdb -lm -std=c++14 -fopenmp
INCLUDE=-I/usr/local/include -I./include
LIBRARY=-L/usr/local/lib
OBJS=obj/sim.o obj/simulator.o obj/event.o obj/evlist.o obj/lp.o obj/communicator.o
TARGET=sim

$(TARGET):$(OBJS)
		    $(CXX) $^ -o $@ $(CXXFLAGS) $(LIBRARY) $(INCLUDE)
obj/sim.o:src/sim.cc
		    $(CXX) $< -o $@ -c $(CXXFLAGS) $(LIBRARY) $(INCLUDE)
obj/simulator.o:src/simulator.cc include/simulator.hh
		    $(CXX) $< -o $@ -c $(CXXFLAGS) $(LIBRARY) $(INCLUDE)
obj/event.o:src/event.cc include/event.hh
		    $(CXX) $< -o $@ -c $(CXXFLAGS) $(LIBRARY) $(INCLUDE)
obj/evlist.o:src/evlist.cc include/evlist.hh
		    $(CXX) $< -o $@ -c $(CXXFLAGS) $(LIBRARY) $(INCLUDE)
obj/lp.o:src/lp.cc include/lp.hh
		    $(CXX) $< -o $@ -c $(CXXFLAGS) $(LIBRARY) $(INCLUDE)
obj/communicator.o:src/communicator.cc include/communicator.hh
		    $(CXX) $< -o $@ -c $(CXXFLAGS) $(LIBRARY) $(INCLUDE)
clean:
		${RM} $(TARGET) $(OBJS)
