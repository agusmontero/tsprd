SYSTEM = x86-64_osx
# or x86-64_linux
LIBFORMAT  = static_pic

# Solver
CPLEXDIR = /Applications/CPLEX_Studio129/cplex
CONCERTDIR = /Applications/CPLEX_Studio129/concert

# Dependencies
HOPSCOTCHMAPDIR = _
CONCORDE = ../../common/concorde

# Compiler
CCC = g++
CCOPT = -m64 -O3 -fPIC -fno-strict-aliasing -fexceptions -DIL_STD -Wno-deprecated-declarations -w

# Link options and libraries
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) -L$(CONCORDE)
CCLNFLAGS = -lconcorde -lconcert -lilocplex -lcplex -lm -lpthread -ldl -lboost_system -lboost_filesystem

all:
	make all_cpp

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
HOPSCOTCHMAPINCDIR = $(HOPSCOTCHMAPDIR)/include

CCFLAGS = -std=c++17 $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) -I$(CONCORDE) -I$(HOPSCOTCHMAPINCDIR)

TARGET = main
CPP_EX = main

all_cpp: $(TARGET)

# ------------------------------------------------------------

log.o: ../../common/log/log.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c ../../common/log/log.cpp $(CCLNFLAGS)

tour.o: ../../common/tour.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c ../../common/tour.cpp $(CCLNFLAGS)

stopwatch.o: ../../common/stopwatch.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c ../../common/stopwatch.cpp $(CCLNFLAGS)

reader.o: ../../common/reader.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c ../../common/reader.cpp $(CCLNFLAGS)

repository.o: ../../common/repository.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c ../../common/repository.cpp $(CCLNFLAGS)

network.o: ../../common/network.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c ../../common/network.cpp $(CCLNFLAGS)

experiment.o: experiment.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c experiment.cpp $(CCLNFLAGS)

time_explorer.o: time_explorer.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c time_explorer.cpp $(CCLNFLAGS)

myopic.o: myopic.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c myopic.cpp $(CCLNFLAGS)

waiter.o: waiter.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c waiter.cpp $(CCLNFLAGS)

maxflow_mincut.o: ../../common/gcs/maxflow_mincut.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c ../../common/gcs/maxflow_mincut.cpp $(CCLNFLAGS)

gcs.o: ../../common/gcs/gcs.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c ../../common/gcs/gcs.cpp $(CCLNFLAGS)

solution.o: solution.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c solution.cpp $(CCLNFLAGS)

mip.o: mip.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c mip.cpp $(CCLNFLAGS)

main.o: main.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -c main.cpp $(CCLNFLAGS)

$(TARGET): reader.o repository.o network.o main.o experiment.o \
		log.o stopwatch.o gcs.o maxflow_mincut.o solution.o \
		time_explorer.o mip.o tour.o waiter.o myopic.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o $(TARGET) main.o \
		reader.o repository.o network.o experiment.o \
		log.o stopwatch.o gcs.o maxflow_mincut.o solution.o \
		time_explorer.o mip.o tour.o waiter.o myopic.o $(CCLNFLAGS)

clean :
	/bin/rm -rf *.o *~
	/bin/rm -rf $(CPP_EX)
	/bin/rm -rf *.mps *.ord *.sos *.lp *.sav *.net *.msg *.log *.clp
	/bin/rm -f *.pul *.sav *.res \ .* \ .sol
