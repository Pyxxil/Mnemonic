# Mnemonic
An Assembler and Simulator for the Assembly Language used in CompSci 110 at UoA

# Dependencies
 - cmake
 - git

# How to build/run
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE
make -j`nproc`
./as <file.asm>  # Assemble the file
./si <file.obj>  # Run the object file
```
