// Level2.cpp
#include "Level2.h"

Level2::Level2(wxWindow* parent)
    : LevelBase(parent)
{
    LoadScreen(0);
}

void Level2::LoadScreen(int screen)
{
   m_levelScreen = screen;
    ClearButtons();

    wxString imageFile;
    if (screen == 0) imageFile = "assets/Level2/page_start.png";
    else if (screen == 1) imageFile = "assets/Level2/plan_select.png";
    else if (screen == 2) imageFile = "assets/Level2/plan_select.png";
    else if (screen == 3) imageFile = "assets/Level2/plan_select.png";
    else if (screen == 4) imageFile = "assets/Level2/plan_select.png";
    else {
        wxMessageBox("Level 1 complete!", "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    if (!m_bgImage.LoadFile(imageFile)) {
        // Failed to load -> keep plain grey background
        SetBackgroundColour(*wxLIGHT_GREY);
    } else {
        // Resize the parent frame to match the image size
        if (wxFrame* frame = dynamic_cast<wxFrame*>(GetParent())) {
            wxSize newSize(m_bgImage.GetWidth(), m_bgImage.GetHeight());
            frame->SetClientSize(newSize);    // Resize the window
            SetSize(newSize);                  // Resize the panel to fill the frame
            frame->Layout();                   // Update layout
        }
    }

    // Add buttons for each screen
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
    // 1st Button for add button
    else if (screen == 1)
    {
        wxButton* nextBtn1 = CreateNextButton("Add Plan", wxPoint(1120,330), wxSize(55,20));
        nextBtn1->SetToolTip("Next Screen 2");
        nextBtn1->Bind(wxEVT_BUTTON, [this](wxCommandEvent&){
            if (!m_timingStarted) {
                m_timingStarted = true;
                m_startTimeMs = wxGetLocalTimeMillis();
            }
            LoadScreen(m_levelScreen + 1);
        });
    }
    // 2nd Button for add button
    else if (screen == 2)
    {
        wxButton* nextBtn1 = CreateNextButton("Add Plan", wxPoint(1120,370), wxSize(55,20));
        nextBtn1->SetToolTip("Next Screen 2");
        nextBtn1->Bind(wxEVT_BUTTON, [this](wxCommandEvent&){
            if (!m_timingStarted) {
                m_timingStarted = true;
                m_startTimeMs = wxGetLocalTimeMillis();
            }
            LoadScreen(m_levelScreen + 1);
        });
    }
    // 3rd Button for add button
    else if (screen == 3)
    {
        wxButton* nextBtn1 = CreateNextButton("Add Plan", wxPoint(1120,410), wxSize(55,20));
        nextBtn1->SetToolTip("Next Screen 2");
        nextBtn1->Bind(wxEVT_BUTTON, [this](wxCommandEvent&){
            if (!m_timingStarted) {
                m_timingStarted = true;
                m_startTimeMs = wxGetLocalTimeMillis();
            }
            LoadScreen(m_levelScreen + 1);
        });
    }
    else if (screen == 4)
    {
        int winWidth = std::max(20, m_bgImage.IsOk() ? m_bgImage.GetWidth() : 800);
        int winHeight = std::max(20, m_bgImage.IsOk() ? m_bgImage.GetHeight() : 600);
        int px = std::max(20, winWidth - 135);
        int py = std::max(20, winHeight - 35);
        wxButton* finalBtn = CreateFinalButton("Submit", wxPoint(px, py), wxSize(80,25));
        finalBtn->SetToolTip("Finish Level 2");
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

    Refresh();  // redraw panel
}
