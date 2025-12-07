#include "Level1.h"

Level1::Level1(wxWindow* parent)
    : LevelBase(parent)
{
    LoadScreen(0);
}

void Level1::LoadScreen(int screen)
{
    m_levelScreen = screen;
    ClearButtons();

    wxString imageFile;
    if (screen == 0) imageFile = "assets/Level1/page_start.png";
    else if (screen == 1) imageFile = "assets/Level1/plan_select.png";
    else if (screen == 2) imageFile = "assets/Level1/plan_select.png";
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
        wxButton* nextBtn = CreateNextButton("Continue", wxPoint(30,240), wxSize(90,25));
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
        wxButton* nextBtn1 = CreateNextButton("Add All", wxPoint(1290,245), wxSize(70,40));
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
        finalBtn->SetToolTip("Finish Level 1");
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
