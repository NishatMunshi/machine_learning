all: graphics

clean:
	rm -fr ./*.o
	rm main.exe

graphics: graphics_compile graphics_link graphics_run

graphics_compile:
	E:/programming_tools/msys64/mingw64/bin/g++.exe -std=c++20 -O3 -Wall -c "./main.cpp" "./matrix.cpp" "./neural_network.cpp" "./random_number_generator.cpp" -I"E:\programming_tools\SFML\SFML_Sources\include" -I"./include" -DSFML_STATIC

graphics_link:
	E:/programming_tools/msys64/mingw64/bin/g++.exe -o main "./main.o" "./matrix.o" "./neural_network.o" "./random_number_generator.o" -L"E:\programming_tools\SFML\SFML_Build\lib" -L"E:\programming_tools\SFML\SFML_Sources\extlibs\libs-msvc\x64" -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lsfml-main-d -lopengl32 -lwinmm -lgdi32 -lfreetype

graphics_run:
	.\main