CXX = clang

FLAGS = -Wall -Wextra -Wpedantic -std=c++17

LINK = -lstdc++ -lsfml-window -lsfml-graphics -lsfml-system

WINLINK = -lglew32 -lopengl32

all:
	$(CXX) $(FLAGS) -o diorama.bin diorama.cpp $(LINK) $(WINLINK) 
