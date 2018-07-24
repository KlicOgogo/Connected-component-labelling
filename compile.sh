rm -f -r src/.exe
mkdir src/.exe

g++ -O2 -std=c++14 src/algorithm.cpp src/image_generators.cpp src/main.cpp -o src/.exe/app `pkg-config --cflags --libs opencv`
