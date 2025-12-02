// LevelBase.h
#pragma once
#include <wx/wx.h>
#include <vector>
#include <functional>

class LevelBase : public wxPanel
{
public:
    // Called by MyFrame: when level completes, call this with elapsed ms
    std::function<void(long)> onFinished;

    LevelBase(wxWindow* parent);
    virtual ~LevelBase();

    // load specified screen index (0..n). Child classes implement their own screens.
    virtual void LoadScreen(int screen) = 0;

    // Starts timer right after countdown ends
    void StartRace();

protected:
    wxImage m_bgImage;
    int m_levelScreen = 0;

    // timing
    bool m_timingStarted = false;
    wxLongLong m_startTimeMs = 0;

    std::vector<wxButton*> m_buttons;

    // helpers for children
    wxButton* CreateNextButton(const wxString& label, const wxPoint& pos, const wxSize& size);
    wxButton* CreateFinalButton(const wxString& label, const wxPoint& pos, const wxSize& size);

    void ClearButtons();
    void DrawBackground(wxPaintEvent&);
};
