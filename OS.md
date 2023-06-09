# OS vs Library vs Loader
 
What should generally be provided by an os and what can be done by a library (that is the same on all operating systems). When I say library I always mean an os-independent library. When I say OS i mean the kernel or an OS-native library.

## OS

### Filesystem

Some open, read and write calls on c are probably the easiest way of interacting with a filesystem. Alghough it is unclear weather the buffering should be done by the os or the library. ATM we use the buffering from the OS.

### Time

???

### Random seed

Probably yes

### Networking

Probably yes

### Memory Allocation.

Here there are two options. Have some mmap like function provided by the os and to the allocation ourselves or have malloc/calloc/... provided by the os. ATM we use the latter

### Threading

This seems very complicated. especially Thread local variables.

## Library

### Locale

This is not needed from the os. Maybe the os needs a way of saying what the locale is. But that can be done through an env variable. There is no c functions from the os needed for anything like this.

### String processing

## Loader

### Name resolutions for stack traces

## Compiler

### Make stack trace just with pointer
