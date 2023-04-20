/**
 * @file mmap_fstream.cc
 * @author Caleb Henry
 * @brief Creates an fstream object and allows access to both read and write in a file
 * @version 1.0
 * @date 2023-04-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "./mmap_fstream.h"

// Creates Memory-mapped file stream obj without file name; file name must be
//   specified in call to Open.
//
//   You may use default parameter values to allow following constructors to
//   absorb this one
//
mem_map::fstream::fstream() {
}


// Creates Memory-mapped file stream obj with file name
//
//   Open mode is std::ios_base::in | std::ios_base:: out by default.
//
//   Result of constructor can be determined by is_open
//
//   You may use default parameter values to allow following constructor to
//   absorb this one
//
mem_map::fstream::fstream(const std::string& fname) {
  open(fname);
}


// Creates Memory-mapped file stream obj with file name and open mode
//
//   Result of constructor can be checked with is_open
//
//   Must handle any combination of modes
//     - std::ios_base::ate  (open with cursor at the end of the file)
//     - std::ios_base::in  (open with read privileges)
//     - std::ios_base::out  (open with write privileges)
//
mem_map::fstream::fstream(const std::string& fname,
  std::ios_base::openmode mode) {
  open(fname, mode);
}

// Attempts to open file given by file_name
//
//   Open mode is std::ios_base::in | std::ios_base::out by default
//
//   Result can be determined by is_open
//
//   Does nothing if file is already open
//
//   You may use default parameter values to overload following method to
//   absorb this one
//
//   Simplifying assumption: no file will ever be larger than 2^12 bytes
//
void mem_map::fstream::open(const std::string& fname) {
  int fd;
  alloc = page_size;
  if (access(fname.c_str(), R_OK | W_OK) == -1) {
    fd = ::open(fname.c_str(), O_RDWR | O_CREAT, 0664);
    curr_open = true;
    file_size = 0;
    index = 0;
    file_name = fname;
  } else {
    fd = ::open(fname.c_str(), O_RDWR, 0664);
    curr_open = true;
    struct  stat sb;
    file_size = fstat(fd, &sb);
    file_size = sb.st_size;
    index = 0;
    file_name = fname;
  }
  // MEMORY MAP OPEN FILE
  data = static_cast<char *>(mmap(NULL,
                          alloc,
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED,
                          fd,
                          0));
  ::close(fd);
  // ALLOCATE FILE MEMORY
  truncate(file_name.c_str(), alloc);
}


// Attempts to open file given by name with open mode specified by mode
//
//   Result can be determined by is_open
//
//   Does nothing if file is already open
//
//   Must handle any combination of modes
//     - std::ios_base::ate  (open with cursor at the end of the file)
//     - std::ios_base::in  (open with read privileges)
//     - std::ios_base::out  (open with write privileges)
//
//   Simplifying assumption: no file will ever be larger than 2^12 bytes
//
void mem_map::fstream::open(const std::string& fname,
  std::ios_base::openmode mode) {
  int access_flags = 0;
  int open_flags = 0;
  int prot_flags = 0;
  int fd;
  bool end = false;
  alloc = page_size;

  // Determines the flags for access, open, and mmap
  if ((mode & std::ios_base::in) == std::ios_base::in) {
      access_flags = access_flags | R_OK;
      open_flags = open_flags | O_RDONLY;
      prot_flags = prot_flags | PROT_READ;
  }
  if ((mode & std::ios_base::out) == std::ios_base::out) {
      access_flags = access_flags | W_OK;
      open_flags = open_flags | O_RDWR;
      prot_flags = prot_flags | PROT_WRITE;
  }
  if ((mode & std::ios_base::ate) == std::ios_base::ate) {
      end = true;
  }

  // Checks if a file exists, then either creates it and opens it
  if (access(fname.c_str(), access_flags) == -1) {
    fd = ::open(fname.c_str(), open_flags | O_CREAT, 0664);
    curr_open = true;
    file_size = 0;
    index = 0;
    file_name = fname;
  } else {
    fd = ::open(fname.c_str(), open_flags, 0664);
    curr_open = true;
    struct  stat sb;
    file_size = fstat(fd, &sb);
    file_size = sb.st_size;
    if (end) {
      index = file_size;
    } else {
      index = 0;
    }
    file_name = fname;
  }
  // MEMORY MAP OPEN FILE
  data = static_cast<char *>(mmap(NULL,
                          4096,
                          prot_flags,
                          MAP_SHARED,
                          fd,
                          0));
  ::close(fd);
  // ALLOCATE FILE MEMORY
  truncate(file_name.c_str(), alloc);
}


// Attempts to close an open file
//
//   Does nothing if file is already open
//
//   Simplifying assumption: no file will ever be larger than 2^12 bytes
//
void mem_map::fstream::close() {
  // SAVE TO DISK
  truncate(file_name.c_str(), file_size);
  msync(data, file_size, MS_SYNC);
  curr_open = false;
}


// Returns file's open state
//
bool mem_map::fstream::is_open() const {
  return curr_open;
}


// Returns file's current size; may change dynmically due to fstream::put
//
std::size_t mem_map::fstream::size() const {
  return file_size;
}


// Retrieves "next" character from file and updates cursor
//
//  Returns '\0' when no further characters exist
//
//  This method must not modify a file; only updates cursor position if not
//  at end of file
//
char mem_map::fstream::get() {
  // READ FROM FILE
  if (index < file_size) {
    return data[index++];
  }
  return '\0';
}


// Writes character at "next" space in the file and updates cursor
//
//  This method may increase the size of a file
//
mem_map::fstream& mem_map::fstream::put(char c) {
  // WRITE TO FILE
  if (index < file_size && file_size < alloc) {
    data[index] = c;
    index++;
  } else if (index == file_size && file_size < alloc) {
    data[index] = c;
    index++;
    file_size++;
  }
  return *this;
}
