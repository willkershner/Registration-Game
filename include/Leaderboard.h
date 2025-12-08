#pragma once
#include <wx/wx.h>
#include <vector>

struct LeaderboardEntry {
    wxString name;
    double time;
    int level;
};

class Leaderboard
{
public:
    /**
     * A clean-up function for controlling the outputted json string
     * 
     * @param s inputted string for iteration
     * @return the necessary escape character/hex code for formatting the json string correctly
     */
    static wxString escapeJsonString(const wxString& s);

    // Save a result to the JSON file
    static void SaveResultToJson(const wxString& name, double timeSeconds, int levelIndex);

    // Read entries for a specific level
    static std::vector<LeaderboardEntry> ReadLeaderboardEntries(int levelIndex = -1);

    // Returns a formatted leaderboard string for a given level
    static wxString ReadLeaderboardDisplay(int levelIndex, const wxString& currentPlayer = "");
};
