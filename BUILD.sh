set -e
#sudo apt-get install build-essential clang libc++-dev libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libsdl2-mixer-dev clang
#g++ -Wall -g -Werror main.cc -std=c++0x -lSDL2 -I/usr/include/SDL2 -o a.out
clang++ main.cc -o a.out -std=c++11 -stdlib=libc++ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -I/usr/include/SDL2
./a.out
