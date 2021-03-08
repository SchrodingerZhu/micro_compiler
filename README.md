# Micro Compiler
## Build Dependency
The following environment is tested:
- (Arch) Linux 5.11.3-zen1-1-zen
- CMake 3.18
- G++ 10.2.0
- Ninja 1.10.2

## Run Dependency
The compiler itself is standalone; however, to compile and run the assembly file requires MUSL toolchain and QEMU:

- MUSL MIPSEL (from musl.cc)
- QEMU (qemu-mipsel version 5.2.0)

## Docker
We have already provided you with a docker file.
So you can use:
```
./build.sh
```
to build the docker image which compiles the whole project at the same time.

Then you can use
```
./compiler <input-file>
```
to compile and run the input file.

## Manually Build

It is also possible to manually build the project following the steps:
```
cd <project-directory>
mkdir build
cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release
ninja all
```


## Manually Run
To run the compiler and QEMU simulator:
```
cd <project-directory>/build
./mcompiler -i <input-file> -o output.S
mcc -static output.S -o test
qemu-mipsel ./test
```

## Other Functions
The compiler also support some other functions like print the IR or AST. To play with, you can start from the help:
```
[root@c7104b0d94d9 project]# cd build
[root@c7104b0d94d9 build]# ./mcompiler -h
USAGE:
compiler <input> [OPTIONS]
OPTIONS:
-h,--help           print this help message can return
-o,--output <path>  set output position, use stdout by default
-i,--ir             display/output IR instead of assembly
-a,--ast            display/output AST structure instead of assembly
NOTICE:
- IR and AST mode can be set in same time
- if no input file provided, the compiler will use stdin as the input
```
