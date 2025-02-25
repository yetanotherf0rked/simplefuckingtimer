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

install: simplefuckingtimer
	@echo "Installing simplefuckingtimer to /usr/local/bin..."
	install -Dm755 simplefuckingtimer /usr/local/bin/simplefuckingtimer
	@echo "Installing icon to /usr/share/pixmaps..."
	install -Dm644 assets/sft-logo-256.png /usr/share/pixmaps/sft-logo-256.png
	@echo "Installing desktop file to /usr/share/applications..."
	install -Dm644 simplefuckingtimer.desktop /usr/share/applications/simplefuckingtimer.desktop

uninstall:
	@echo "Removing simplefuckingtimer from /usr/local/bin..."
	rm -f /usr/local/bin/simplefuckingtimer
	@echo "Removing icon from /usr/share/pixmaps..."
	rm -f /usr/share/pixmaps/sft-logo-256.png
	@echo "Removing desktop file from /usr/share/applications..."
	rm -f /usr/share/applications/simplefuckingtimer.desktop

