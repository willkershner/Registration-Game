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
    static wxString escapeJsonString(const wxString& s);

    // Save a result to the JSON file
    static void SaveResultToJson(const wxString& name, double timeSeconds, int levelIndex);

    // Read entries for a specific level
    static std::vector<LeaderboardEntry> ReadLeaderboardEntries(int levelIndex = -1);

    // Returns a formatted leaderboard string for a given level
    static wxString ReadLeaderboardDisplay(int levelIndex, const wxString& currentPlayer = "");
};
