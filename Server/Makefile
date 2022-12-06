CXX = g++
CXXFLAGS = -Wall
LDLIBS = -lpthread

PROGRAMS = Server
OBJS = Server.o errorFun.o

all: $(PROGRAMS)

$(PROGRAMS) : $(OBJS)
	$(CXX) -o $@ $^ $(LDLIBS)
  
%.o : %.c
	$(CXX) -c $< -o $@

clean:
	rm -f $(PROGRAMS)
	rm -rf *.o