File 1: makefile
    Purpose: Creates and runs the tests on the fstream object.
    test-all: Runs all tests
    test-open: Opens an fstream object and checks if the size variable intializes correctly
    test-get: Opens an fstream object and tests if the get function works
    test-put: Opens an fstream object and tests if the put function works

File 2: mmap_fstream.h
    Purpose: The header file for mmap_fstream.cc

File 3: mmap_fstream.cc
    Purpose: Creates an fstream object and allows access to both read and write in a file

File 4: test_mmap_fstream.cc
    Purpose: Holds tests for each of the functions in mmap_fstream