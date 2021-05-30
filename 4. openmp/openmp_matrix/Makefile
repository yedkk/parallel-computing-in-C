CC = g++
CFLAGS = -I. -O3 -mavx -fopenmp
DEPS = hw4_part2.hpp
OBJ = hw4_part2.o
LIB = lib_hw4_part2.a

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(LIB): $(OBJ)
	ar rcs $@ $(OBJ)
	
.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB)
