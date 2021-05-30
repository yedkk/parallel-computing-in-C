CC=g++
CFLAGS= -I. -O3 -fopenmp
OBJ = cannon_omp.o
LIB = cannon_omp.a

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB): $(OBJ)
	ar rcs $@ $(OBJ)

.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB)
