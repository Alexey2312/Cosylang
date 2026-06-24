clear:
    rm -rf build

run:
    ./build/Cosylang

build:
    mkdir -p build
    cmake -B build -G Ninja
    cmake --build build
