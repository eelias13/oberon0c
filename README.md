# A Compiler for the Oberon-0 Programming Language

The [Oberon](https://www.ethoberon.ethz.ch) programming language was proposed in 1987 by 
[Niklaus Wirth](https://people.inf.ethz.ch/wirth/) as a successor to Pascal and Modula-2. Due to this lineage, Oberon 
is an ALGOL-like language with strong (static and dynamic) typing discipline. The programming paradigm of Oberon can 
be classified as imperative, structured, modular, and object-oriented.

## About

This project implements a compiler for the Oberon programming language as a frontend to the [LLVM](http://llvm.org)
Compiler Infrastructure. It is written in C++ and originated as project accompanying the MSc course "Compiler 
Construction" taught at the [University of Konstanz](https://uni.kn). As a consequence, this compiler originally only
targeted the Oberon-0 subset of the language, as described in Niklaus Wirth's book 
["Compiler Construction"](http://www.ethoberon.ethz.ch/WirthPubl/CBEAll.pdf) (Chapter 6, pp. 30-32). Since then, the
supported subset of the Oberon has been continuously extended with the goal to eventually cover the full language 
specification as described in the latest version of the [Oberon Language Report](https://inf.ethz.ch/personal/wirth/Oberon/Oberon07.Report.pdf).
In addition to these "official" extensions, other features were added to the supported dialect of the Oberon programming language.
These feature were either inspired by convenience, such as interfacing with standard libraries, or by the compiler 
author's nostalgia of learning [Turbo Pascal](https://en.wikipedia.org/wiki/Turbo_Pascal) 6.0 as his first programming 
language at high school.

## Dependencies and Toolchains

Owing to its origin as a course project, care has been taken that the provided C++ sourcecode can be compiled on
different operating systems and with different toolchains. Currently, the sourcecode only depends on 
[Boost](https://www.boost.org) and [LLVM](https://llvm.org).
As of December 2020, the following configurations are tested and known to work.

|      | macOS                       | Windows 10 (x64)             | Linux (Ubuntu 20.04.1 LTS)   |
|------|-----------------------------|------------------------------|------------------------------|
|Boost | 1.74.0                      | 1.74.0                       | 1.71.0                       |
|LLVM  | 11.0.0                      | 11.0.0                       | 11.0.0                       |
|CMake | 3.19.1                      | 3.19.2                       | 3.19.2                       |
|IDE   | CLion 2020.3                | Visual Studio 2019 Community | CLion 2020.3                 |
|CXX   | Apple Clang 12.0.0.12000032 | Microsoft CL 19.24.28316     | GCC 9.3.0                    |

## Installing Boost

### macOS

Using [Homebrew](https://brew.sh), Boost can be installed as follows.
```
> brew install boost
```

### Linux

### Windows

It is recommended to use Microsoft's [vcpkg](https://github.com/microsoft/vcpkg) C++ Library Manager for Windows in order to install the required Boost components.

1. Install vcpkg by following the steps below.

   ```
   > git clone https://github.com/Microsoft/vcpkg.git
   > cd vcpkg
   > .\bootstrap-vcpkg.bat
   ```

2. Configure automatic, user-wide integration of libraries managed by vcpkg with Visual Studio 2019.

   ```
   > .\vcpkg integrate install
   ```

2. Use vcpkg to install the required Boost libraries (`system`, `convert`, `filesystem`, and `program_options`).

   ```
   > .\vcpkg install boost-system:x64-windows
   > .\vcpkg install boost-convert:x64-windows
   > .\vcpkg install boost-filesystem:x64-windows
   > .\vcpkg install boost-program-options:x64-windows
   ```

## Installing LLVM

### macOS
```
> brew install llvm
```

### Linux

### Windows

Using the Microsoft C++ Library Manager is the simplest way to install LLVM on Windows (x64). There are, however, some drawbacks. First, vcpkg will compile and install both the release and the debug version of LLVM, which requires 70-80 GB of free harddisk space during installation. Second, it typically takes a while before a new release of LLVM is integrated into the vcpkg distribution. Should any of these two factors present a problem, please refer to the instructions given below on how to build LLVM from the sources.

Assuming, you already followed the steps described above to install Boost using vcpkg, LLVM can be compiled and installed with the following command.

   ```
   > .\vcpkg install llvm:x64-windows
   ```
 


