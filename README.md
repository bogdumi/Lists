# Lists

A reusable C library for module for doubly linked lists. The modulee is not thread safe. The header is set up to store int values in lists, but item is used as a synonym for int everywhere, so that there is only one place where a change would need to be made to store some other type of items. This means the module can be used with different item types in different programs, but it is not truly generic. It cannot be used multiple times for different item types in the same program. There is no really satisfying way of making fully generic modules in C.

## Install

A Makefile is included in the repo with the necessary flags, and it will work on Windows, MacOS and any Linux distro, so just type `make` in the console in the folder with the `lists.c` source file and `lists.h` header file.

## Testing

Run the compiled program with no arguments to test the module with the in-built tests:

- `./lists`

