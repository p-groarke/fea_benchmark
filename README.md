# cppBenchUtil
Some benchmarks and benchmarking helper functions.

##### Instructions
Simply include benchUtil.h in your program. It is a collection of functions inside `Bench` namespace.

```
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
Optional but really recommended string took: 0.000002s
```

##### Compiling
Of course, use your compiler optimizations when testing performance.

###### Windows
`cl /O2 main.cpp`

###### OS X
`clang++ -O3 -std=c++11 -stdlib=libc++ main.cpp`
