// Level3.cpp
#include "Level3.h"
#include <wx/msgdlg.h>
#include <wx/button.h>
#include <wx/timer.h>
#include <random>

Level3::Level3(wxWindow* parent)
    : LevelBase(parent)
{
    LoadScreen(0);
}

// ------------------------------------------------------
// LOADING ONLY (NO WIFI OUTAGE) – SHOW LOADING IMAGE
// ------------------------------------------------------
void Level3::SimulateLoadingOnly(std::function<void()> onFinished)
{
    m_isLoading = true;
    m_bgImage.LoadFile("assets/Level3/loading.png");

    // Resize to loading image
    if (m_bgImage.IsOk())
    {
        if (wxFrame* frame = dynamic_cast<wxFrame*>(GetParent()))
        {
            wxSize newSize(m_bgImage.GetWidth(), m_bgImage.GetHeight());
            frame->SetClientSize(newSize);
            SetSize(newSize);
            frame->Layout();
        }
    }

    Refresh();
    Update();

    // Random loading duration (3–10 sec)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(3, 10);
    int delaySec = dist(gen);

    wxTimer* timer = new wxTimer(this);
    Bind(wxEVT_TIMER, [=](wxTimerEvent&)
    {
        timer->Stop();
        delete timer;

        m_isLoading = false;

        if (onFinished)
            onFinished();

    }, timer->GetId());

    timer->Start(delaySec * 1000, wxTIMER_ONE_SHOT);
}

// ------------------------------------------------------
// WIFI OUTAGE – ***NO LOADING SCREEN***
// ------------------------------------------------------
void Level3::SimulateWifiOutage(std::function<void()> onFinished)
{
    // First popup
    wxMessageDialog dlg1(
        this,
        "Your Wi-Fi connection has been lost.\n\nPlease wait...",
        "Wi-Fi Disconnected",
        wxOK | wxICON_WARNING
    );
    dlg1.ShowModal();

    // Random outage duration (3–10 seconds)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(3, 10);
    int delaySec = dist(gen);

    // No loading screen — user cannot click anything anyway
    m_isLoading = true;

    wxTimer* timer = new wxTimer(this);
    Bind(wxEVT_TIMER, [=](wxTimerEvent&)
    {
        timer->Stop();
        delete timer;

        // Wi-Fi restored popup
        wxMessageDialog dlg2(
            this,
            "Wi-Fi connection restored.",
            "Wi-Fi Reconnected",
            wxOK | wxICON_INFORMATION
        );
        dlg2.ShowModal();

        m_isLoading = false;

        if (onFinished)
            onFinished();

    }, timer->GetId());

    timer->Start(delaySec * 1000, wxTIMER_ONE_SHOT);
}

// ------------------------------------------------------
// MAIN SCREEN LOADING
// ------------------------------------------------------
void Level3::LoadScreen(int screen)
{
    m_levelScreen = screen;
    ClearButtons();

    wxString imageFile;

    if (screen == 0)
        imageFile = "assets/Level3/page_start.png";
    else if (screen == 1)
        imageFile = "assets/Level3/plan_select.png";
    else if (screen == 2)
        imageFile = "assets/Level3/plan_select.png";
    else
    {
        wxMessageBox("Level 3 complete!", "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    // Load image & resize window
    if (!m_bgImage.LoadFile(imageFile))
    {
        SetBackgroundColour(*wxLIGHT_GREY);
    }
    else
    {
        if (wxFrame* frame = dynamic_cast<wxFrame*>(GetParent()))
        {
            wxSize newSize(m_bgImage.GetWidth(), m_bgImage.GetHeight());
            frame->SetClientSize(newSize);
            SetSize(newSize);
            frame->Layout();
        }
    }

    // ---------------- Screen 0 ----------------
    if (screen == 0)
    {
        wxButton* nextBtn = CreateNextButton("Continue", wxPoint(30,240), wxSize(90,25));

        nextBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
        {
            if (!m_timingStarted)
            {
                m_timingStarted = true;
                m_startTimeMs = wxGetLocalTimeMillis();
            }
            LoadScreen(1);
        });
    }

    // ---------------- Screen 1 ----------------
    else if (screen == 1)
    {
        wxButton* addPlan = CreateNextButton(
            "Add Plan",
            wxPoint(1290,245), 
            wxSize(70,40)
        );

        addPlan->Bind(wxEVT_BUTTON, [this, addPlan](wxCommandEvent&)
        {
            // Disable the button IMMEDIATELY after clicking
            addPlan->Disable();

            if (!m_timingStarted)
            {
                m_timingStarted = true;
                m_startTimeMs = wxGetLocalTimeMillis();
            }

            // RANDOM CHOICE:
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(1, 2);
            int eventType = dist(gen);

            if (eventType == 1)
            {
                SimulateWifiOutage([this]()
                {
                    LoadScreen(2);
                });
            }
            else
            {
                SimulateLoadingOnly([this]()
                {
                    LoadScreen(2);
                });
            }
        });
    }


    // ---------------- Screen 2 ----------------
    else if (screen == 2)
    {
        int winWidth  = m_bgImage.IsOk() ? m_bgImage.GetWidth()  : 800;
        int winHeight = m_bgImage.IsOk() ? m_bgImage.GetHeight() : 600;

        int px = winWidth  - 135;
        int py = winHeight - 35;

        wxButton* finalBtn = CreateFinalButton(
            "Submit", wxPoint(px, py), wxSize(80,25)
        );

        finalBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
        {
            if (!m_timingStarted)
                m_startTimeMs = wxGetLocalTimeMillis();

            long elapsedMs = (wxGetLocalTimeMillis() - m_startTimeMs).ToLong();
            if (elapsedMs < 0) elapsedMs = 0;

            if (onFinished)
                onFinished(elapsedMs);
        });
    }

    Refresh();
}
