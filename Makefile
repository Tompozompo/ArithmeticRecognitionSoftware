CC=gcc
CXX=g++
FLAGS=-I./src/ -g `pkg-config --cflags opencv`
TARGETS=finalex
LIBS=-lm `pkg-config --libs opencv` -lstdc++
OBJ=lib/ARS.o lib/finalex.o lib/find_closest.o lib/train_cvknearest.o

main: $(TARGETS)

$(TARGETS): $(OBJ) 
	$(CC) $(FLAGS) -o $(TARGETS) $^ $(LIBS)

lib/finalex.o: src/finalex.c
	$(CC) $(FLAGS) -c -o lib/finalex.o $< $(LIBS)

lib/ARS.o: src/ARS.c
	$(CC) $(FLAGS) -c -o lib/ARS.o $< $(LIBS)

lib/find_closest.o: src/find_closest.cpp
	$(CXX) $(FLAGS) -c -o lib/find_closest.o $< $(LIBS)

lib/train_cvknearest.o: src/train_cvknearest.cpp
	$(CXX) $(FLAGS) -c -o lib/train_cvknearest.o $<  $(LIBS)

clean:
	rm -rf $(TARGETS) lib/*.o

create_samples: src/create_samples.c
	$(CC) $(FLAGS) -o create_sample $< $(LIBS)
