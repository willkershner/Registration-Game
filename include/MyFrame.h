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

    void ShowLanding();
    void ShowCountdown();
    void ShowLevel(int levelIndex);
    void ShowResults(long elapsedMs);

private:
    wxPanel* m_currentPanel = nullptr;

    wxString playerName;
    int selectedLevel = 0;

    // helpers
    void ReplacePanel(wxPanel* newPanel);
};
