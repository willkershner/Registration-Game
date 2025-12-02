// ResultsScreen.cpp
#include "ResultsScreen.h"
#include "Leaderboard.h"

ResultsScreen::ResultsScreen(wxWindow* parent, long elapsedMs, const wxString& playerName)
    : wxPanel(parent), m_playerName(playerName), m_elapsedMs(elapsedMs)
{
    SetBackgroundColour(*wxWHITE);
    SetClientSize(600, 400);

    double seconds = m_elapsedMs / 1000.0;

    wxString msg;
    msg.Printf("You finished in %.3f seconds!", seconds);

    wxStaticText* resultText = new wxStaticText(this, wxID_ANY, msg, wxPoint(50, 120), wxSize(500, 40), wxALIGN_CENTRE_HORIZONTAL);
    resultText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    wxStaticText* savedText = new wxStaticText(this, wxID_ANY, "Saving your result...", wxPoint(50, 170), wxSize(500, 30), wxALIGN_CENTRE_HORIZONTAL);

    // Save to JSON
    Leaderboard::SaveResultToJson(m_playerName, seconds);
    savedText->SetLabel("Your score has been saved to leaderboard.json");

    // Back button
    wxButton* back = new wxButton(this, wxID_ANY, "Back to Menu", wxPoint(220, 260), wxSize(160, 40));
    back->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        if (onBack) onBack();
    });

    ShowLeaderboard();

    Refresh();
}

void ResultsScreen::ShowLeaderboard()
{
    wxString display = Leaderboard::ReadLeaderboardDisplay();
    if (display.IsEmpty()) return;

    wxTextCtrl* leaderboardCtrl = new wxTextCtrl(this, wxID_ANY, display,
                                                 wxPoint(50, 220), wxSize(500, 150),
                                                 wxTE_MULTILINE | wxTE_READONLY);
}
