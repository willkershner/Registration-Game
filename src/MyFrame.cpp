// MyFrame.cpp
#include "MyFrame.h"
#include "LandingPage.h"
#include "Countdown.h"
#include "LevelBase.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "ResultsScreen.h"

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Game Landing Page",
              wxPoint(20, 50), wxSize(800, 600),
              wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
#if defined(_WIN32)
    // optional: per-monitor DPI awareness on Windows
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

    // Add image handlers
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);

    m_currentPanel = nullptr;
    ShowLanding();
}

MyFrame::~MyFrame()
{
    if (m_currentPanel) m_currentPanel->Destroy();
}

void MyFrame::ReplacePanel(wxPanel* newPanel)
{
    if (!newPanel) return;

    if (!GetSizer()) {
        SetSizer(new wxBoxSizer(wxVERTICAL));
    }

    // Remove and destroy old panel if it exists
    if (m_currentPanel) {
        GetSizer()->Detach(m_currentPanel);  // remove from sizer
        m_currentPanel->Hide();
        m_currentPanel->Destroy();
    }

    m_currentPanel = newPanel;

    // Add new panel to sizer
    GetSizer()->Add(m_currentPanel, 1, wxEXPAND);
    m_currentPanel->Show();

    Layout();
}



void MyFrame::ShowLanding()
{
    auto landing = new LandingPage(this);
    landing->onStart = [this](const wxString& name, int level){
        playerName = name;
        selectedLevel = level;
        ShowCountdown();
    };
    ReplacePanel(landing);
}

void MyFrame::ShowCountdown()
{
    auto c = new Countdown(this);
    c->onFinished = [this](){
        ShowLevel(selectedLevel);
    };
    ReplacePanel(c);
}

void MyFrame::ShowLevel(int levelIndex)
{
    LevelBase* lvl = nullptr;
    switch (levelIndex)
    {
        case 0: lvl = new Level1(this); break;
        case 1: lvl = new Level2(this); break;
        case 2: lvl = new Level3(this); break;
        default: lvl = new Level1(this); break;
    }

    // When a level finishes it will call onFinished with elapsed ms
    lvl->onFinished = [this](long ms){
        ShowResults(ms);
    };

    ReplacePanel(lvl);

    // Start the race immediately after countdown
    lvl->StartRace();
}

void MyFrame::ShowResults(long elapsedMs)
{
    auto results = new ResultsScreen(this, elapsedMs, playerName, selectedLevel);

    results->onBack = [this](){
        playerName.clear();
        selectedLevel = 0;
        ShowLanding();  // show main menu panel
    };

    ReplacePanel(results);
}
