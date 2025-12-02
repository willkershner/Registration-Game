# Simple Makefile for wxWidgets Game
# Assumes:
#   src/*.cpp
#   include/*.h
#   assets/*

WXFLAGS = `wx-config --cxxflags --libs`
INCLUDES = -Iinclude

# -----------------------------
# Default target
# -----------------------------
all:
	make game
	make clean_after

# -----------------------------
# Link the final executable
# -----------------------------
game: src/main1.cpp MyApp.o MyFrame.o LandingPage.o Countdown.o LevelBase.o Level1.o Level2.o Level3.o ResultsScreen.o Leaderboard.o
	g++ -o game src/main1.cpp MyApp.o MyFrame.o LandingPage.o Countdown.o LevelBase.o Level1.o Level2.o Level3.o ResultsScreen.o Leaderboard.o $(WXFLAGS) $(INCLUDES)

# -----------------------------
# Object file rules
# -----------------------------
MyApp.o: src/MyApp.cpp include/MyApp.h
	g++ -c src/MyApp.cpp -o MyApp.o $(WXFLAGS) $(INCLUDES)

MyFrame.o: src/MyFrame.cpp include/MyFrame.h
	g++ -c src/MyFrame.cpp -o MyFrame.o $(WXFLAGS) $(INCLUDES)

LandingPage.o: src/LandingPage.cpp include/LandingPage.h
	g++ -c src/LandingPage.cpp -o LandingPage.o $(WXFLAGS) $(INCLUDES)

Countdown.o: src/Countdown.cpp include/Countdown.h
	g++ -c src/Countdown.cpp -o Countdown.o $(WXFLAGS) $(INCLUDES)

LevelBase.o: src/LevelBase.cpp include/LevelBase.h
	g++ -c src/LevelBase.cpp -o LevelBase.o $(WXFLAGS) $(INCLUDES)

Level1.o: src/Level1.cpp include/Level1.h
	g++ -c src/Level1.cpp -o Level1.o $(WXFLAGS) $(INCLUDES)

Level2.o: src/Level2.cpp include/Level2.h
	g++ -c src/Level2.cpp -o Level2.o $(WXFLAGS) $(INCLUDES)

Level3.o: src/Level3.cpp include/Level3.h
	g++ -c src/Level3.cpp -o Level3.o $(WXFLAGS) $(INCLUDES)

ResultsScreen.o: src/ResultsScreen.cpp include/ResultsScreen.h
	g++ -c src/ResultsScreen.cpp -o ResultsScreen.o $(WXFLAGS) $(INCLUDES)

Leaderboard.o: src/Leaderboard.cpp include/Leaderboard.h
	g++ -c src/Leaderboard.cpp -o Leaderboard.o $(WXFLAGS) $(INCLUDES)

# -----------------------------
# Clean up
# -----------------------------
clean_after:
	rm -f *.o

clean:
	rm -f *.o game
