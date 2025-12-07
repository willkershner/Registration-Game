#pragma once
#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/image.h>
#include <functional>

class Countdown : public wxPanel
{
public:
    Countdown(wxWindow* parent);

    // Callback when countdown finishes
    std::function<void()> onFinished;

private:
    void OnTick(wxTimerEvent&);
    void OnPaint(wxPaintEvent&);

    wxTimer m_timer;
    wxStaticText* countdownLabel = nullptr;

    // Count-up timer members
    int hours = 6;
    int minutes = 59;
    int seconds = 55;

    // Background image
    wxImage m_bgImage;

    wxDECLARE_EVENT_TABLE();
};
