# Define variables
CC = gcc
CFLAGS = -fdiagnostics-color=always -g
M_SRC = main.c  #main src
T_SRC = ./tests/test.c  #test src

SRC = $(wildcard libs/*.c)
OUTPUT = ./main
T_OUTPUT = ./tests/test

# Default target
build:
# Rule to build the output
	$(CC) $(CFLAGS) $(M_SRC) $(SRC) -o $(OUTPUT)

# Run the program
run: $(OUTPUT)
	 $(OUTPUT)

#build and run
bnr: build run

#test file run
test_build:
	$(CC) $(CFLAGS) $(T_SRC) $(SRC) -o $(T_OUTPUT)

# Run the program
test_run: $(T_OUTPUT)
	 $(T_OUTPUT)

#build and run
testNrun: test_build test_run

# Clean up build files
clean:
	rm -f $(OUTPUT)
	clear

# Clean up test build files
testClean:
	rm -f $(T_OUTPUT)
	clear
