#include "ResultsScreen.h"
#include "Leaderboard.h"

ResultsScreen::ResultsScreen(wxWindow* parent, long elapsedMs, const wxString& playerName, int levelIndex)
    : wxPanel(parent), m_playerName(playerName), m_elapsedMs(elapsedMs), m_levelIndex(levelIndex)
{
    SetBackgroundColour(*wxWHITE);

    // Resize the parent frame
    if (wxFrame* frame = dynamic_cast<wxFrame*>(GetParent())) {
        frame->SetClientSize(600, 400);
        SetSize(frame->GetClientSize());
        frame->Layout();
    }

    double seconds = m_elapsedMs / 1000.0;
    wxString msg;
    msg.Printf("You finished in %.3f seconds!", seconds);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Result message
    wxStaticText* resultText = new wxStaticText(this, wxID_ANY, msg, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    resultText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(resultText, 0, wxALIGN_CENTER | wxTOP, 50);

    // Saving message
    wxStaticText* savedText = new wxStaticText(this, wxID_ANY, "Saving your result...", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    mainSizer->Add(savedText, 0, wxALIGN_CENTER | wxTOP, 20);

    // Save the result
    Leaderboard::SaveResultToJson(m_playerName, seconds, m_levelIndex);
    savedText->SetLabel("Your score has been saved!");

    // Leaderboard display
    std::vector<LeaderboardEntry> entries = Leaderboard::ReadLeaderboardEntries(m_levelIndex);
    if (!entries.empty()) {
        wxTextCtrl* leaderboardCtrl = new wxTextCtrl(this, wxID_ANY, "",
                                                     wxDefaultPosition, wxSize(500, 150),
                                                     wxTE_MULTILINE | wxTE_READONLY);
        
        leaderboardCtrl->SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        // Append entries with current player in red
        for (const auto& e : entries) {
            long start = leaderboardCtrl->GetLastPosition();
            wxString line;
            line.Printf("%s : %.3f sec\n", e.name, e.time);
            leaderboardCtrl->AppendText(line);

            if (e.name == m_playerName) {
                wxTextAttr attr;
                attr.SetTextColour(*wxRED);
                leaderboardCtrl->SetStyle(start, leaderboardCtrl->GetLastPosition(), attr);
            }
        }

        mainSizer->Add(leaderboardCtrl, 0, wxALIGN_CENTER | wxTOP, 20);
    }

    // Back to menu button
    wxButton* back = new wxButton(this, wxID_ANY, "Back to Menu", wxDefaultPosition, wxSize(160, 40));
    back->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        if (onBack) onBack();
    });
    mainSizer->Add(back, 0, wxALIGN_CENTER | wxTOP, 20);

    SetSizer(mainSizer);
    Layout();
    Refresh();
}
