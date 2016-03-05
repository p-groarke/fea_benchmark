# cppBenchUtil
Some benchmarks and benchmarking helper functions.

###### Instructions
Simply include benchUtil.h in your program. It is a collection of functions inside `Bench` namespace.

```
Bench::title("Your title");
Bench::start("Optional string");
// Do stuff
Bench::end("Optional but really recommended string");
```

Will output:

```
----------
Your title
----------
Optional string
Optional but really recommended string took: 0.000002s
```
