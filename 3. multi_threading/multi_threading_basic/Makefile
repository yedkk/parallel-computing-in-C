CC=g++
CFLAGS= -I. -O3 
DEPS = hw3_part1.h
OBJ = hw3_part1.o
LIB = lib_sd_thread.a

%.o: %.cpp $(DEPS)
	$(CC) -std=c++11 -c -o $@ $< $(CFLAGS)

$(LIB): $(OBJ)
	ar rcs $@ $(OBJ)

.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB)
