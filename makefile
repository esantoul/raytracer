all:
	mkdir -p exec
	g++ main.cpp -O3 -std=c++17 -Wall -Wextra -Wpedantic -Werror -o exec/raytracer
	./exec/raytracer
	ffmpeg -y -i scenes/test.ppm scenes/test.png
