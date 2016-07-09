kompilator - SPL compiler
=========================

Introduction
------------

kompilator is a compiler for SPL (Simple Programming Language) written in C++.
kompilator is a work in progress. It is my first C++ project, as well as my first attempt at writing a compiler.

Building
--------

```
mkdir build
cd build
cmake ..
make
```

Running
-------

```
build/src/kompilator res/spl.grammar tests/0.spl
```

Credits
-------

The definition of SPL has been taken from the Compiler Construction course of Radboud University Nijmegen, The Netherlands.
Tests 0 to 23 were borrowed from https://github.com/Jille/splparser.
