#include <wx/wx.h>
#include <wx/image.h>
#include <vector>
#include <fstream>

#if defined(_WIN32)
#include <windows.h>
#endif

class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Game Landing Page",
                  wxPoint(20, 50),
                  wxSize(800, 600),
                  wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
    {
#if defined(_WIN32)
        SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        ShowLandingPage();
    }

private:
    wxPanel* panel = nullptr;
    wxImage bgImage;

    ofstream leaderboard;

    // Landing page controls
    wxStaticText* nameLabel = nullptr;
    wxTextCtrl* nameInput = nullptr;
    wxChoice* levelChoice = nullptr;
    std::vector<wxButton*> landingButtons;

    // Level 1 state
    int level1Screen = 0;
    std::vector<wxBitmapButton*> levelButtons;

    int winWidth = 800;
    int winHeight = 600;

    /******** LANDING PAGE ********/
    void ShowLandingPage()
    {
        if (panel) panel->Destroy();
        panel = new wxPanel(this);
        panel->SetBackgroundColour(*wxLIGHT_GREY);  // Gray background
        panel->SetDoubleBuffered(true);

        // Label for name
        nameLabel = new wxStaticText(panel, wxID_ANY, "Enter your name:", wxPoint(300, 200), wxSize(200, 30));
        nameLab el->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        // Name input
        nameInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 230), wxSize(200, 30));

        // Level selection
        wxArrayString levels;
        levels.Add("Level 1");
        levels.Add("Level 2");
        levelChoice = new wxChoice(panel, wxID_ANY, wxPoint(300, 270), wxSize(200, 30), levels);
        levelChoice->SetSelection(0);

        // Start button
        wxButton* startButton = new wxButton(panel, wxID_ANY, "Start Game", wxPoint(300, 320), wxSize(200, 40));
        startButton->Bind(wxEVT_BUTTON, &MyFrame::OnStartClicked, this);
        landingButtons.push_back(startButton);

        // Example: Another button (like "Help")
        wxButton* helpButton = new wxButton(panel, wxID_ANY, "Help", wxPoint(300, 380), wxSize(200, 40));
        helpButton->Bind(wxEVT_BUTTON, [](wxCommandEvent&)
        {
            wxMessageBox("This is the help message.", "Help", wxOK | wxICON_INFORMATION);
        });
        landingButtons.push_back(helpButton);

        panel->Bind(wxEVT_PAINT, &MyFrame::OnPaint, this);
        panel->Refresh();
    }

    void OnStartClicked(wxCommandEvent&)
    {
        static wxString playerName = nameInput->GetValue();
        int selectedLevel = levelChoice->GetSelection();

        if (playerName.IsEmpty())
        {
            wxMessageBox("Please enter your name!", "Info", wxOK | wxICON_INFORMATION);
            return;
        }

        // Remove landing page controls
        if (nameLabel) { nameLabel->Destroy(); nameLabel = nullptr; }
        if (nameInput) { nameInput->Destroy(); nameInput = nullptr; }
        if (levelChoice) { levelChoice->Destroy(); levelChoice = nullptr; }
        for (auto btn : landingButtons) btn->Destroy();
        landingButtons.clear();

        // Start Level 1 multi-screen
        if (selectedLevel == 0)
            LoadLevel1Screen(0);
        else
            wxMessageBox("Level 2 not implemented yet!", "Info", wxOK | wxICON_INFORMATION);
    }

    /******** LEVEL 1 MULTI-SCREEN ********/
    void ClearLevelButtons()
    {
        for (auto btn : levelButtons) btn->Destroy();
        levelButtons.clear();
    }

    void LoadLevel1Screen(int screen){
    level1Screen = screen;
    ClearLevelButtons();

    // Determine which background image to load
    wxString imageFile;
    if (screen == 0) imageFile = "page_start.png";
    else if (screen == 1) imageFile = "plan_select.png";
    else if (screen == 2) imageFile = "submit.png";
    else
    {
        wxMessageBox("Level 1 complete!", "Info", wxOK | wxICON_INFORMATION);

        updateLeaderboard(playerName);

        return;
    }

    if (!bgImage.LoadFile(imageFile))
    {
        wxMessageBox("Failed to load " + imageFile, "Error", wxOK | wxICON_ERROR);
        return;
    }

    winWidth = bgImage.GetWidth();
    winHeight = bgImage.GetHeight();
    SetSize(winWidth, winHeight);
    SetSizeHints(wxSize(winWidth, winHeight), wxSize(winWidth, winHeight));

    wxBitmap emptyBmp(150, 50);
    {
        wxMemoryDC dc(emptyBmp);
        dc.SetBackground(*wxTRANSPARENT_BRUSH);
        dc.Clear();
        dc.SelectObject(wxNullBitmap);
    }

    // --- Screen-specific buttons ---
    if (screen == 0)
    {
        // First button for screen 0
        wxBitmapButton* nextBtn = new wxBitmapButton(panel, wxID_ANY, emptyBmp, wxPoint(30, 240), wxSize(150, 50), wxBORDER_NONE);
        nextBtn->SetToolTip("Next");
        nextBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
        {
            LoadLevel1Screen(level1Screen + 1); // advance to screen 1
        });
        levelButtons.push_back(nextBtn);

        // // Another button for screen 0
        // wxBitmapButton* extraBtn0 = new wxBitmapButton(panel, wxID_ANY, emptyBmp, wxPoint(200, 240), wxSize(150, 50), wxBORDER_NONE);
        // extraBtn0->SetToolTip("Extra 0");
        // extraBtn0->Bind(wxEVT_BUTTON, [](wxCommandEvent&)
        // {
        //     wxMessageBox("Extra button on screen 0 clicked!", "Info", wxOK | wxICON_INFORMATION);
        // });
        // levelButtons.push_back(extraBtn0);
    }
    else if (screen == 1)
    {
        // Button for screen 1
        wxBitmapButton* nextBtn1 = new wxBitmapButton(panel, wxID_ANY, emptyBmp, wxPoint(50, 250), wxSize(150, 50), wxBORDER_NONE);
        nextBtn1->SetToolTip("Next Screen 2");
        nextBtn1->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
        {
            LoadLevel1Screen(level1Screen + 1); // advance to screen 2
        });
        levelButtons.push_back(nextBtn1);

        // Another button for screen 1
        wxBitmapButton* extraBtn1 = new wxBitmapButton(panel, wxID_ANY, emptyBmp, wxPoint(250, 250), wxSize(150, 50), wxBORDER_NONE);
        extraBtn1->SetToolTip("Extra 1");
        extraBtn1->Bind(wxEVT_BUTTON, [](wxCommandEvent&)
        {
            wxMessageBox("Extra button on screen 1 clicked!", "Info", wxOK | wxICON_INFORMATION);
        });
        levelButtons.push_back(extraBtn1);
    }
    else if (screen == 2)
    {
        // Buttons for screen 2
        wxBitmapButton* finalBtn = new wxBitmapButton(panel, wxID_ANY, emptyBmp, wxPoint(100, 300), wxSize(150, 50), wxBORDER_NONE);
        finalBtn->SetToolTip("Finish Level 1");
        finalBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
        {
            wxMessageBox("Level 1 complete!", "Info", wxOK | wxICON_INFORMATION);
        });
        levelButtons.push_back(finalBtn);
    }

    panel->Refresh();
}

    /******** PAINT EVENT ********/
    void OnPaint(wxPaintEvent&)
    {
        wxPaintDC dc(panel);
        dc.Clear();

        if (bgImage.IsOk())
            dc.DrawBitmap(wxBitmap(bgImage), 0, 0, false);
    }

    /******** RECORD DATA ********/
    void updateLeaderboard(wxString playerName)
    {
        leaderboard.open("leaderboard.txt");
        leaderboard << playerName << endl;
        leaderboard.close();
    }
};

/******** MAIN APP ********/
class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        MyFrame* frame = new MyFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
