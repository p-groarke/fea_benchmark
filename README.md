# cpp_bench_util
Some benchmarks and benchmarking helper functions.

## Instructions
Simply include bench_util.h in your program. It is a collection of functions inside `bench` namespace.

### Cmake
You can build all examples and install the header in your user include path easily.
```
mkdir build
cd build
cmake ..
make
make install
```

### Usage
```
#include "../bench_util.h"

[...]

bench::title("Your title");
bench::start("Optional string");
// Do stuff
bench::clobber();
bench::end("Optional but really recommended string");

bench::start();
// Do stuff
bench::clobber();
bench::end("Optional but really recommended string");
```

outputs:

```
##########
Your title
##########


Optional string
---------------
Optional but really recommended string                                0.000000s
Optional but really recommended string                                0.000000s
```

### Compiling
Of course, use your compiler optimizations when testing performance.

###### Windows
`cl /O2 main.cpp`

###### OS X
`clang++ -O3 -std=c++11 -stdlib=libc++ main.cpp`

### Useful commands
In most cases, you want to inspect various program attributes to understand the results. Here are a some useful commands.

###### Windows
- Generate assembly
  - /FA Assembly code; .asm
  - /FAc Machine and assembly code; .cod
  - /FAs Source and assembly code; .asm
  - /FAcs Machine, source, and assembly code; .cod
- Vtable layout
  - `cl /d1reportAllClassLayout main.cpp`
  - `cl /d1reportSingleClassLayoutYOURCLASS main.cpp`

###### OS X
- Generate assembly
  - `clang++ -S -std=c++11 -stdlib=libc++ -mllvm --x86-asm-syntax=intel main.cpp`
  - `clang++ -S -std=c++11 -stdlib=libc++ -masm=intel main.cpp` (Clang 3.5+)
- Vtable layout
  - `clang++ -cc1 -std=c++11 -stdlib=libc++ -fdump-record-layouts main.cpp`
- Memory layout (vtable + objects)
  - `clang++ -std=c++11 -stdlib=libc++ -Xclang -fdump-record-layouts -fsyntax-only main.cpp`
- Vtable with LLVM IR
  - `clang++ -std=c++11 -stdlib=libc++ -Xclang -fdump-record-layouts main.cpp`
- Dissasembly
  - `otool -tv a.out`

[More clang commands](http://clang.llvm.org/docs/CommandGuide/clang.html)

###### Linux
- Generate assembly
  - `g++ -S main.cpp`
  - `g++ -Wa,-adhln -g main.cpp` (Assembly + Source)
- Vtable layout
  - `g++ -fdump-class-hierarchy main.cpp`
- Dissasembly
  - `objdump -d a.out`

### Tools
In no specific order, various tools to profile results.

- [Zoom](http://www.rotateright.com) (Windows, Mac, Linux)
- [Valgrind Suite (Cachegrind, Memcheck)](http://valgrind.org/info/tools.html) (Mac, Linux)
- [Visual Studio Profiling Tools](https://msdn.microsoft.com/en-us/library/bb385770.aspx) (Windows)
- [Xcode Instruments](https://developer.apple.com/library/watchos/documentation/DeveloperTools/Conceptual/InstrumentsUserGuide/index.html) (Mac)
- [perf](https://perf.wiki.kernel.org/index.php/Main_Page) (Linux)
- [OProfile](http://oprofile.sourceforge.net) (Linux)
- [Intel Vtune](https://software.intel.com/en-us/intel-vtune-amplifier-xe) (Commercial - Windows, Linux)

[More tools](https://en.wikipedia.org/wiki/List_of_performance_analysis_tools)
