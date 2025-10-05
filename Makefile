CXX = g++
CXXFLAGS = -std=c++17 -ID:/SFML-2.6.2/include
LDFLAGS = -LD:/SFML-2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SOURCES = CaroGame.cpp Page.cpp Home.cpp HowToPlay.cpp ClassicModeMenu.cpp BombMode.cpp menu1.cpp menu2.cpp popup.cpp PvP_Classic.cpp PvE_Classic.cpp bomb.cpp PvP_bomb.cpp PvE_bomb.cpp bot.cpp move.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	# Use 'rm -f' for Unix-like shells (common in MinGW/MSYS2 environments)
	rm -f *.o *.exe
