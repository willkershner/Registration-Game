# Simple Makefile for wxWidgets Game
# Assumes:
#   src/*.cpp
#   include/*.h
#   assets/*

CXX      = g++
CXXFLAGS = -std=c++17 -Iinclude $(shell wx-config --cxxflags)
LDFLAGS  = $(shell wx-config --libs)

# -----------------------------
# Default target
# -----------------------------
all:
	make game
	make clean_after

# compile main.cpp
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# link everything
game: main.o MyApp.o MyFrame.o LandingPage.o Countdown.o LevelBase.o Level1.o Level2.o Level3.o ResultsScreen.o Leaderboard.o
	$(CXX) -std=c++17 -o game main.o MyApp.o MyFrame.o LandingPage.o Countdown.o LevelBase.o Level1.o Level2.o Level3.o ResultsScreen.o Leaderboard.o $(LDFLAGS)

MyFrame.o: src/MyFrame.cpp include/MyFrame.h
	$(CXX) $(CXXFLAGS) -c src/MyFrame.cpp -o MyFrame.o

MyApp.o: src/MyApp.cpp include/MyApp.h
	$(CXX) $(CXXFLAGS) -c src/MyApp.cpp -o MyApp.o

LandingPage.o: src/LandingPage.cpp include/LandingPage.h
	$(CXX) $(CXXFLAGS) -c src/LandingPage.cpp -o LandingPage.o

Countdown.o: src/Countdown.cpp include/Countdown.h
	$(CXX) $(CXXFLAGS) -c src/Countdown.cpp -o Countdown.o

LevelBase.o: src/LevelBase.cpp include/LevelBase.h
	$(CXX) $(CXXFLAGS) -c src/LevelBase.cpp -o LevelBase.o

Level1.o: src/Level1.cpp include/Level1.h
	$(CXX) $(CXXFLAGS) -c src/Level1.cpp -o Level1.o

Level2.o: src/Level2.cpp include/Level2.h
	$(CXX) $(CXXFLAGS) -c src/Level2.cpp -o Level2.o

Level3.o: src/Level3.cpp include/Level3.h
	$(CXX) $(CXXFLAGS) -c src/Level3.cpp -o Level3.o

ResultsScreen.o: src/ResultsScreen.cpp include/ResultsScreen.h
	$(CXX) $(CXXFLAGS) -c src/ResultsScreen.cpp -o ResultsScreen.o

Leaderboard.o: src/Leaderboard.cpp include/Leaderboard.h
	$(CXX) $(CXXFLAGS) -c src/Leaderboard.cpp -o Leaderboard.o

# -----------------------------
# Clean up
# -----------------------------
clean_after:
	rm -f *.o

clean:
	rm -f *.o game
