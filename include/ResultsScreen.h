// ResultsScreen.h
#pragma once
#include <wx/wx.h>
#include <functional>

class ResultsScreen : public wxPanel
{
public:
    std::function<void()> onBack;
    ResultsScreen(wxWindow* parent, long elapsedMs, const wxString& playerName, int levelIndex);

private:
    void ShowLeaderboard();

    wxString m_playerName;
    long m_elapsedMs;
    int m_levelIndex;
};
