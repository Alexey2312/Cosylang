clear:
    rm -rf build

run:
    ./build/Cosylang

build:
    mkdir build
    cmake -B build -G Ninja
    cmake --build build
    just run

clean-build:
    just clear
    just build
    just clear

debug-build:
    mkdir build
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    just run

use-clang:
    just clear
    clang++ -std=c++23 -g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer $(find . -name '*.cpp' -print0 | xargs -0) -o clangbuild
    ./clangbuild
