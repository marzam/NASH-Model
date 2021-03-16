################################################################################
#
#
#  Universidade Federal Rural do Rio de Janeiro
#  Instituto Multidisciplinar - Departamento de Ciencia da Computacao
#  por: Marcelo Zamith - zamith.marcelo@gmail.com
#  para compilar com target release use o comando: make target=release + rápido
#
#  versão de on-ramp
#
################################################################################
EXEFILE      = TModelCA++
APPLICATION  = -D_APPLICATION=\"$(EXEFILE)\"
#CPUCC       = /opt/intel/bin/icpc
#CPUCC       =  /opt/gnu-gcc/bin/g++
CPUCC       =  g++
CPPFLAGS    = -m64 -fopenmp -Wno-reorder #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS        = $(APPLICATION)  $(VERSION) -DALIGN=64
INCLUDES  =  -I.
LIBDIR      = #-L/opt/intel/lib/intel64
#LIBDIR      = -L/opt/intel/lib/intel64
#LIBDIR      = -L/opt/gnu-gcc/lib64
LIBS     =  -lm
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)

ifeq ($(target), release)
  DEFS      +=
  CPPFLAGS  +=      -O3
else
  DEFS     += -D_DEBUG
  CPPFLAGS += -g -Wall -O0
endif

all:	CellularAutomata 	\
		Configure       \
        main

	$(CPU_COMPILE)  obj/CellularAutomata.o         \
			        obj/Configure.o        \
			        obj/main.o             \
	$(LINK) -o $(EXEFILE)

main:
	$(CPU_COMPILE) -c main.cpp -o obj/main.o

Configure:
	$(CPU_COMPILE) -c Configure.cpp -o obj/Configure.o

CellularAutomata:
	$(CPU_COMPILE) -c CellularAutomata.cpp -o obj/CellularAutomata.o

clean:
	rm *.o

files:
	rm *.txt; rm *.dat; rm *.o
