// LandingPage.cpp
#include "LandingPage.h"

LandingPage::LandingPage(wxWindow* parent)
    : wxPanel(parent)
{
    SetBackgroundColour(*wxLIGHT_GREY);
    SetDoubleBuffered(true);

    int winWidth = 800, winHeight = 600;
    parent->SetClientSize(winWidth, winHeight);

    // Name label
    nameLabel = new wxStaticText(this, wxID_ANY, "Enter your name:", wxPoint(300, 200), wxSize(200, 30));
    nameLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // Name input
    nameInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(300, 230), wxSize(200, 30));

    // Level selection
    wxArrayString levels;
    levels.Add("Level 1");
    levels.Add("Level 2");
    levels.Add("Level 3");
    levelChoice = new wxChoice(this, wxID_ANY, wxPoint(300, 270), wxSize(200, 30), levels);
    levelChoice->SetSelection(0);

    // Start button
    wxButton* startButton = new wxButton(this, wxID_ANY, "Start Game", wxPoint(300, 320), wxSize(200, 40));
    startButton->Bind(wxEVT_BUTTON, &LandingPage::OnStartClicked, this);
    m_buttons.push_back(startButton);

    // Help button
    wxButton* helpButton = new wxButton(this, wxID_ANY, "Help", wxPoint(300, 380), wxSize(200, 40));
    helpButton->Bind(wxEVT_BUTTON, [](wxCommandEvent&)
    {
        wxMessageBox("This is a simulation of class registration. Each level adds another condition that can affect the process. To play the game follow the steps below:\n1. Enter your name.\n2. Select a level.\n3. Click 'Start Game' to begin.\n4. Click the buttons until you click submit.\n5. Compare your time versus your peers.", "Help", wxOK | wxICON_INFORMATION);
    });
    m_buttons.push_back(helpButton);

    Bind(wxEVT_PAINT, [this](wxPaintEvent&){ wxPaintDC dc(this); dc.Clear(); });
}

void LandingPage::OnStartClicked(wxCommandEvent&)
{
    wxString playerName = nameInput->GetValue();
    int selectedLevel = levelChoice->GetSelection();  // 0,1,2

    if (playerName.IsEmpty()) {
        wxMessageBox("Please enter your name!", "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    if (onStart) onStart(playerName, selectedLevel);
}
