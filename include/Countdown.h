#pragma once
#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/image.h>
#include <functional>

class Countdown : public wxPanel
{
public:
    /**
     * Initializes the countdown structure, including the grey rectangle that covers the screen
     * 
     * @param parent a pointer to the window in which the countdown intervenes.
     */
    Countdown(wxWindow* parent);

    // Callback when countdown finishes
    std::function<void()> onFinished;

private:
    /**
     * An event handler class for controlling behavior as the timer counts down.
     * 
     * @param wxTimerEvent a reference of the timer event, which is a wxEvent-derived class used to control time-based outputs
    */
    void OnTick(wxTimerEvent&);

    /**
     * An event handler class for controlling behavior at the time of repainting.
     * 
     * @param wxPaintEvent a reference of the paint event, which is a wxEvent-derived class instantiated when the window must be repainted
     */
    void OnPaint(wxPaintEvent&);

    wxTimer m_timer;
    wxStaticText* countdownLabel = nullptr;

    // Count-up timer members
    int hours = 6;
    int minutes = 59;
    int seconds = 55;

    // Background image
    wxImage m_bgImage;

    /**
     * A macro that clarifies this class is one that uses an event table.
     */
    wxDECLARE_EVENT_TABLE();
};
