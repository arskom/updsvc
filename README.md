# Update Service

Windows update service for your projects


## Build instructions

Install  VS Community with C++ capability and CMake support. 
Then launch a developer shell:

```bat
cd L:\path\to\local\clone
md build-Debug
cd build-Debug
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug ..
ninja
```
