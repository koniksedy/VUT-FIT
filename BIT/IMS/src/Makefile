# Makefile pro přeložení projektu do IMS
# Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>

CXX=g++
CFLAGS=-g -O2
LIBS=-lm
# Použijte buď lokální simlib.so, nebo na merlinovi knihovnu -lsimlib
SIMLIB=../lib/simlib.so
DEPENDENCIES=deliveryCar.o lavicky.o getGoodsFromTruck.o materialGenerator.o jobSelector.o woodPrepare.o woodPaint.o woodRepair.o assemblyController.o machine.o assembling.o welding.o


all: lavicky

lavicky: $(DEPENDENCIES)
	$(CXX) $(CFLAGS) $(DEPENDENCIES) $(SIMLIB) -o $@ -lm

# Vytvoří oběktový soubor pro každoý modul
%.o: %.cpp %.hpp
	$(CXX) $(CFLAGS) $< -c -lm

# Spustí program
run: lavicky
	./lavicky

# Vymazání spustitelného souboru a metasouborů kompilace
clean:
	rm -f lavicky $(DEPENDENCIES)
