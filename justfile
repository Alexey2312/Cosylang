clear:
    rm -rf build

run-tests:
    ./build/Cosylang test

build:
    mkdir -p build
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build build

debug-build:
    mkdir -p build
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
