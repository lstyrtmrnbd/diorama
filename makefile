CXX = clang

FLAGS = -Wall -Wextra -Wpedantic -std=c++17

# cast to GLvoid* necessary for glVertexAttribPointer call
SUPPRESS = -Wno-int-to-void-pointer-cast

LINK = -lstdc++ -lsfml-window -lsfml-graphics -lsfml-system

WINLINK = -lglew32 -lopengl32

all:
	$(CXX) $(FLAGS) $(SUPPRESS) -o diorama.bin src/diorama.cpp $(LINK) $(WINLINK) 
