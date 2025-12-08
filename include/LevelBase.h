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

    /**
     * Initializes the LevelBase framework for each of the levels that inherit from it
     * 
     * @param parent the parent window to which all of the levels are offshoots from
     */
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

    /**
     * Function for drawing a new background. (May have been made irrelevant)
     * 
     * @param wxPaintEvent a reference of the paint event, which is a wxEvent-derived class instantiated when the window must be repainted
     */
    void DrawBackground(wxPaintEvent&);
};
