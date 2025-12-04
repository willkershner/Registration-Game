// Level3.cpp
#include "Level3.h"

Level3::Level3(wxWindow* parent)
    : LevelBase(parent)
{
    LoadScreen(0);
}

void Level3::LoadScreen(int screen)
{
    m_levelScreen = screen;
    ClearButtons();

    wxString imageFile;
    if (screen == 0) imageFile = "page_start.png";
    else if (screen == 1) imageFile = "plan_select.png";
    else if (screen == 2) imageFile = "plan_select.png";
    else {
        wxMessageBox("Level 3 complete!", "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    if (!m_bgImage.LoadFile(imageFile)) {
        // ignore
    } else {
        SetClientSize(m_bgImage.GetWidth(), m_bgImage.GetHeight());
    }

    if (screen == 0)
    {
        wxButton* nextBtn = CreateNextButton("Submit", wxPoint(30,240), wxSize(85,25));
        nextBtn->SetToolTip("Next");
        nextBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
            if (!m_timingStarted) {
                m_timingStarted = true;
                m_startTimeMs = wxGetLocalTimeMillis();
            }
            LoadScreen(m_levelScreen + 1);
        });
    }
    else if (screen == 1)
    {
        wxButton* nextBtn1 = CreateNextButton("Add Plan", wxPoint(1200,240), wxSize(150,50));
        nextBtn1->SetToolTip("Next Screen 2");
        nextBtn1->Bind(wxEVT_BUTTON, [this](wxCommandEvent&){
            if (!m_timingStarted) {
                m_timingStarted = true;
                m_startTimeMs = wxGetLocalTimeMillis();
            }
            LoadScreen(m_levelScreen + 1);
        });
    }
    else if (screen == 2)
    {
        int winWidth = std::max(20, m_bgImage.IsOk() ? m_bgImage.GetWidth() : 800);
        int winHeight = std::max(20, m_bgImage.IsOk() ? m_bgImage.GetHeight() : 600);
        int px = std::max(20, winWidth - 135);
        int py = std::max(20, winHeight - 35);
        wxButton* finalBtn = CreateFinalButton("Submit", wxPoint(px, py), wxSize(80,25));
        finalBtn->SetToolTip("Finish Level 3");
        finalBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&){
            if (!m_timingStarted) {
                m_startTimeMs = wxGetLocalTimeMillis();
            } 
            wxLongLong endTime = wxGetLocalTimeMillis();
            long elapsedMs = (endTime - m_startTimeMs).ToLong();
            if (elapsedMs < 0) elapsedMs = 0;
            if (onFinished) onFinished(elapsedMs);
        });
    }

    Refresh();
}
