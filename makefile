CXX = clang

FLAGS = -Wall -Wextra -Wpedantic -std=c++17

# cast to GLvoid* necessary for some gl calls
SUPPRESS = -Wno-int-to-void-pointer-cast

LINK = -lstdc++ -lsfml-window -lsfml-graphics -lsfml-system

ifeq ($(OS), Windows_NT)
	LINK += -lglew32 -lopengl32
else
	LINK += -lGLEW -lGL
endif

all:
	$(CXX) $(FLAGS) $(SUPPRESS) -o diorama.bin src/diorama.cpp $(LINK)
