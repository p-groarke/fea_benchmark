# cppBenchUtil
Some benchmarks and benchmarking helper functions.

### Instructions
Simply include benchUtil.h in your program. It is a collection of functions inside `Bench` namespace.

```
#include "../benchUtil.h"

[...]

Bench::title("Your title");
Bench::start("Optional string");
// Do stuff
Bench::end("Optional but really recommended string");
```

outputs:

```
----------
Your title
----------
Optional string
Optional but really recommended string took: 0.000000s
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
  - `clang++ -S -mllvm --x86-asm-syntax=intel main.cpp`
  - `clang++ -S -masm=intel main.cpp` (Clang 3.5+)
- Vtable layout
  - `clang++ -cc1 -fdump-record-layouts main.cpp`
- Memory layout (vtable + objects)
  - `clang++ -std=c++11 -stdlib=libc++ -Xclang -fdump-record-layouts -fsyntax-only main.cpp`
- Vtable with LLVM IR
  - `clang++ -std=c++11 -stdlib=libc++ -Xclang -fdump-record-layouts main.cpp`

[More clang commands](http://clang.llvm.org/docs/CommandGuide/clang.html)

###### Linux
- Generate assembly
  - `g++ -S main.cpp`
  - `g++ -Wa,-adhln -g main.cpp` (Assembly + Source)
- Vtable layout
  - `g++ -fdump-class-hierarchy main.cpp`
