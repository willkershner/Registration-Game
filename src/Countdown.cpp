// Countdown.cpp
#include "Countdown.h"

wxBEGIN_EVENT_TABLE(Countdown, wxPanel)
    EVT_TIMER(wxID_ANY, Countdown::OnTick)
wxEND_EVENT_TABLE()

Countdown::Countdown(wxWindow* parent)
    : wxPanel(parent), m_timer(this)
{
    // SetBackgroundColour(*wxLIGHT_GREY);
    SetDoubleBuffered(true);

    
    // int margin = 10;
    int labelW = 180, labelH = 28;

    // Make panel grey like landing page
    SetBackgroundColour(*wxLIGHT_GREY);

    // Set window size to match landing page (or default)
    int margin = 10;
    int winWidth = 800;
    int winHeight = 600;
    SetClientSize(winWidth, winHeight);


    countdownValue = 3;
    countdownLabel = new wxStaticText(this, wxID_ANY,
                                      wxString::Format("Starting in %d...", countdownValue),
                                      wxPoint(winWidth - labelW - margin, margin),
                                      wxSize(labelW, labelH), wxALIGN_RIGHT);
    countdownLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // disable parent so user cannot interact with underlying UI (here it's a standalone panel)
    Disable();

    // start timer
    m_timer.Start(1000);
}

void Countdown::OnTick(wxTimerEvent&)
{
    countdownValue--;
    if (countdownValue > 0) {
        if (countdownLabel && countdownLabel->IsShown())
            countdownLabel->SetLabel(wxString::Format("Starting in %d...", countdownValue));
    } else {
        if (m_timer.IsRunning()) m_timer.Stop();
        if (countdownLabel) { countdownLabel->Destroy(); countdownLabel = nullptr; }
        Enable();
        if (onFinished) onFinished();
    }
}
