// LevelBase.cpp
#include "LevelBase.h"

LevelBase::LevelBase(wxWindow* parent)
    : wxPanel(parent)
{
    SetBackgroundColour(*wxWHITE);
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &LevelBase::DrawBackground, this);
}

LevelBase::~LevelBase()
{
    ClearButtons();
}

void LevelBase::StartRace()
{
    if (!m_timingStarted) {
        m_timingStarted = true;
        m_startTimeMs = wxGetLocalTimeMillis();

        // If derived classes used a Start button handler, call it here:
        // For example, if Level1 had a "Start" button logic:
        // OnStartButtonClicked(wxCommandEvent());
    }
}

wxButton* LevelBase::CreateNextButton(const wxString& label, const wxPoint& pos, const wxSize& size)
{
    wxButton* b = new wxButton(this, wxID_ANY, label, pos, size, wxNO_BORDER);
    b->SetBackgroundColour(wxColour(200,200,200));
    m_buttons.push_back(b);
    return b;
}

wxButton* LevelBase::CreateFinalButton(const wxString& label, const wxPoint& pos, const wxSize& size)
{
    wxButton* b = new wxButton(this, wxID_ANY, label, pos, size, wxNO_BORDER);
    b->SetBackgroundColour(wxColour(0,200,0));
    m_buttons.push_back(b);
    return b;
}

void LevelBase::ClearButtons()
{
    for (auto p : m_buttons) if (p) p->Destroy();
    m_buttons.clear();
}

void LevelBase::DrawBackground(wxPaintEvent&)
{
    wxPaintDC dc(this);
    dc.Clear();
    if (m_bgImage.IsOk()) {
        dc.DrawBitmap(wxBitmap(m_bgImage), 0, 0, false);
    }
}
