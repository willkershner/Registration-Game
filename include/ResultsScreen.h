// ResultsScreen.h
#pragma once
#include <wx/wx.h>
#include <functional>

class ResultsScreen : public wxPanel
{
public:
    std::function<void()> onBack;
    /**
     * Initializes the results screen with the data taken from the player's playtime, the landing page, and the leaderboard
     * 
     * @param parent the parent window from which this particular window is an offshoot in the app
     * @param elapsedMs the elapsed time, in milliseconds, that it took the player to complete the game
     * @param playerName the player's name, as read by the leaderboard
     * @param levelIndex the level of the game that the player completed, as read by the landing page
     */
    ResultsScreen(wxWindow* parent, long elapsedMs, const wxString& playerName, int levelIndex);

private:
    void ShowLeaderboard();

    wxString m_playerName;
    long m_elapsedMs;
    int m_levelIndex;
};
