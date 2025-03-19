# A Compiler for the Oberon-0 Programming Language

## About This Project

This project is a compiler for the [Oberon-0](https://oberon.org/en) programming language, originally developed by [Max Galetskiy](https://github.com/max-galetskiy) and [Elias Maier](https://github.com/eelias13) as part of the "Compiler Construction" course at the [University of Konstanz](https://www.uni-konstanz.de/en/). The original compiler supports was written as a command-line app (CLI) and uses llvm for code generation. 
In this repo I extend the project by add a WebAssembly (WASM) version that can be executed in the browser.

## About Oberon-0

Oberon-0 is a simplified subset of the [Oberon](https://oberon.org/en) programming language, created by [Niklaus Wirth](https://people.inf.ethz.ch/wirth/) as a successor to Pascal and Modula-2. It is a strongly-typed, imperative language that emphasizes modularity and simplicity. The goal of this project is to provide a functional Oberon-0 compiler for different execution environments.

## Compilation Targets

### CLI Version

The CLI version of the compiler transforms Oberon-0 source code into LLVM Intermediate Representation (LLVM IR), which can be further compiled into executable binaries.

#### Dependencies

To build the CLI version, the following dependencies are required:
- [Boost](https://www.boost.org/) (for general utility functions)
- [CMake](https://cmake.org/) (for project configuration and build management)
- [LLVM](https://llvm.org/) (for intermediate representation handling)

#### Build Instructions

```bash
mkdir build && cd build
cmake ..
make
```

This will produce the `oberon0c` executable.

### WebAssembly Version

The WebAssembly version of the compiler enables execution within a browser or other WASM runtimes.

#### Dependencies

To build the WASM version, the following dependencies are required:
- [Boost](https://www.boost.org/) (use installation below)
- [Make](https://www.gnu.org/software/make/)
- [Emscripten](https://emscripten.org/) (for compiling to WASM)

#### Install Boost

```bash
wget -O boost.tar.xz https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-b2-nodocs.tar.xz
tar xf boost.tar.xz 
mv boost-1.86.0 ./boost
rm boost.tar.xz
```

Then, export the environment variable:

```bash
export BOOST_ROOT=./boost
```

#### Install Emscripten
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

#### Compile to WebAssembly

```bash
mkdir build && cd build
emcmake cmake ..
make
```

This will produce the `wasm_lib.wasm` and `wasm_lib.js` files needed to run the compiled Oberon-0 programs in a browser or a WASM runtime.

### Running the WebAssembly Compiler
To test the compiled WebAssembly module, open `index.html` in a browser, or run:
```bash
node wasm_lib.js
```

## Authors and Acknowledgements
- **Max Galetskiy**: Co-author of the compiler.
- **Elias Maier**: Co-author of the compiler, developing the WebAssembly target.
- **Michael Grossniklaus**: Provided the initial skeleton and scanner as part of the University of Konstanz course.

For the another direction of the same project, see [max-galetskiy/oberon0c](https://github.com/max-galetskiy/oberon0c).

