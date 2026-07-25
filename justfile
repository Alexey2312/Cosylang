clear:
    rm -rf build

run:
    ./build/Cosylang

build:
    mkdir -p build
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build build
