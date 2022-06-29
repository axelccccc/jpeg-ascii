# jpeg-ascii

A simple command-line JPEG viewer in ASCII graphics.

It uses the [libjpeg-turbo](https://github.com/libjpeg-turbo/libjpeg-turbo) library for JPEG decompression.

Compilation
===========

> The program uses some UNIX system headers and won't compile on Windows.  
> This will probably get fixed in the future.

The program is compiled using the [conan](https://conan.io) package manager and CMake.
```bash
mkdir build && cd build && conan install && cmake .. && make
```

Usage
=====
```
jpeg-ascii <filename.jpg/jpeg>
```