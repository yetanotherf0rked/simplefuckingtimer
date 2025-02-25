CXX = g++
CXXFLAGS = -std=c++11 -O2 -Iinclude
LIBS = -lSDL2 -lSDL2_ttf -lX11 -pthread

SRC = src/main.cpp src/timer.cpp src/hotkeys.cpp
OBJ = $(SRC:.cpp=.o)

all: simplefuckingtimer

simplefuckingtimer: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LIBS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) simplefuckingtimer

