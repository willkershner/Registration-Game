// Countdown.h
#pragma once
#include <wx/wx.h>
#include <functional>

class Countdown : public wxPanel
{
public:
    std::function<void()> onFinished;
    Countdown(wxWindow* parent);

private:
    wxTimer m_timer;
    int countdownValue = 3;
    wxStaticText* countdownLabel = nullptr;

    void Start();
    void OnTick(wxTimerEvent&);
    wxDECLARE_EVENT_TABLE();
};
