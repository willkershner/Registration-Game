// MyFrame.h
#pragma once
#include <wx/wx.h>
#include <memory>

class LandingPage;
class Countdown;
class LevelBase;
class ResultsScreen;

class MyFrame : public wxFrame
{
public:
    MyFrame();
    ~MyFrame();

    /**
     * Shows the landing page
     */
    void ShowLanding();

    /**
     * Shows the countdown overlay
     */
    void ShowCountdown();

    /**
     * Shows the difficulty level requested by the player
     * 
     * @param levelIndex the difficulty level, as read by the landing page
     */
    void ShowLevel(int levelIndex);

    /**
     * Shows the results screen that is outputted to the json leaderboard
     * 
     * @param elapsedMs the amount of time, in milliseconds, that it took the player to complete the game
     */
    void ShowResults(long elapsedMs);

private:
    wxPanel* m_currentPanel = nullptr;

    wxString playerName;
    int selectedLevel = 0;

    // helpers
    void ReplacePanel(wxPanel* newPanel);
};
