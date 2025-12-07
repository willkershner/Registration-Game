#include "Countdown.h"

wxBEGIN_EVENT_TABLE(Countdown, wxPanel)
    EVT_TIMER(wxID_ANY, Countdown::OnTick)
    EVT_PAINT(Countdown::OnPaint)
wxEND_EVENT_TABLE()

Countdown::Countdown(wxWindow* parent)
    : wxPanel(parent), m_timer(this)
{
    SetDoubleBuffered(true);

    int labelW = 180, labelH = 28;
    int margin = 10;
    int winWidth = 800;
    int winHeight = 600;

    int verticalOffset = 100;

    // -------------------------------------------------
    // Load background image
    // -------------------------------------------------
    wxString imageFile = "assets/Level1/page_start.png"; // <-- replace with your path
    if (!m_bgImage.LoadFile(imageFile)) {
        // Failed to load -> grey background
        SetBackgroundColour(*wxLIGHT_GREY);
        SetClientSize(winWidth, winHeight);
    } else {
        // Resize parent frame and panel to match image
        if (wxFrame* frame = dynamic_cast<wxFrame*>(GetParent())) {
            wxSize newSize(m_bgImage.GetWidth(), m_bgImage.GetHeight());
            frame->SetClientSize(newSize);
            SetSize(newSize);
            frame->Layout();
        }
    }

    winWidth = m_bgImage.GetWidth();
    winHeight = m_bgImage.GetHeight();

    // -------------------------------------------------
    // Countdown label
    // -------------------------------------------------
    countdownLabel = new wxStaticText(this, wxID_ANY,
                                      wxString::Format("%02d:%02d:%02d", hours, minutes, seconds),
                                      wxPoint(winWidth - labelW - verticalOffset, verticalOffset),
                                      wxSize(labelW, labelH), wxALIGN_RIGHT);

    // Font: larger and dark
    countdownLabel->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    countdownLabel->SetForegroundColour(wxColour(255, 255, 255));

    // Disable parent so user cannot interact
    Disable();

    // Start timer: tick every second
    m_timer.Start(1000);
}

// -------------------------------------------------
// Draw background image
// -------------------------------------------------
void Countdown::OnPaint(wxPaintEvent&)
{
    wxPaintDC dc(this);

    if (m_bgImage.IsOk())
    {
        wxBitmap bmp(m_bgImage);
        dc.DrawBitmap(bmp, 0, 0, false);
    }
    else
    {
        dc.SetBrush(*wxLIGHT_GREY_BRUSH);
        dc.DrawRectangle(GetClientRect());
    }
}

// -------------------------------------------------
// Timer tick
// -------------------------------------------------
void Countdown::OnTick(wxTimerEvent&)
{
    // Increment one second
    seconds++;
    if (seconds >= 60)
    {
        seconds = 0;
        minutes++;
    }
    if (minutes >= 60)
    {
        minutes = 0;
        hours++;
    }

    // Update label
    if (countdownLabel && countdownLabel->IsShown())
        countdownLabel->SetLabel(wxString::Format("%02d:%02d:%02d", hours, minutes, seconds));

    // Redraw panel to show background
    Refresh();

    // Check if we reached 7:00:00
    if (hours == 7 && minutes == 0 && seconds == 0)
    {
        if (m_timer.IsRunning()) m_timer.Stop();
        if (countdownLabel) { countdownLabel->Destroy(); countdownLabel = nullptr; }
        Enable();
        if (onFinished) onFinished();
    }
}
