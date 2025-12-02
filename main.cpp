// main.cpp
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/timer.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

// If Windows DPI awareness desired
#if defined(_WIN32)
#include <windows.h>
#endif

class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Game Landing Page",
                  wxPoint(20, 50), wxSize(800, 600),
                  wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
          m_countdownTimer(this)
    {
#if defined(_WIN32)
        // optional: per-monitor DPI awareness on Windows
        SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        panel = nullptr;
        nameLabel = nullptr;
        nameInput = nullptr;
        levelChoice = nullptr;
        countdownLabel = nullptr;

        timingStarted = false;
        countdownValue = 3; // seconds

        ShowLandingPage();
    }

    ~MyFrame()
    {
        if (m_countdownTimer.IsRunning())
            m_countdownTimer.Stop();
    }

private:
    // UI
    wxPanel* panel;
    wxImage bgImage;

    // Landing page controls
    wxStaticText* nameLabel;
    wxTextCtrl* nameInput;
    wxChoice* levelChoice;
    std::vector<wxButton*> landingButtons;

    // Level controls/state
    int levelScreen = 0;
    int selectedLevelGlobal = 0;
    std::vector<wxButton*> levelButtons;

    // Timer / countdown
    wxTimer m_countdownTimer;
    int countdownValue;
    wxStaticText* countdownLabel;

    // Timing measurement
    bool timingStarted;
    wxLongLong startTimeMs;
    wxLongLong endTimeMs;
    wxString playerNameGlobal;

    int winWidth = 800;
    int winHeight = 600;

    /****************** Utility: simple JSON writer (writes a root array) ******************/
    static wxString escapeJsonString(const wxString& s)
    {
        wxString out;
        for (size_t i = 0; i < s.length(); ++i) {
            wxChar c = s[i];
            switch (c) {
            case '\"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (c < 0x20) {
                    // control char
                    wxString hex;
                    hex.Printf("\\u%04x", (int)c);
                    out += hex;
                } else {
                    out += c;
                }
            }
        }
        return out;
    }

    void SaveResultToJson(const wxString& name, double timeSeconds)
    {
        const char* filename = "leaderboard.json";

        // Build entry string
        std::ostringstream entry;
        entry << "{";
        entry << "\"name\":\"" << std::string(escapeJsonString(name).mb_str()) << "\",";
        entry.setf(std::ios::fixed); entry << std::setprecision(3);
        entry << "\"time\":" << timeSeconds;
        entry << "}";

        std::ifstream inFile(filename, std::ios::in);
        if (!inFile.good()) {
            // File doesn't exist: create array with the entry
            std::ofstream outFile(filename, std::ios::out);
            outFile << "[" << entry.str() << "]\n";
            outFile.close();
            return;
        }

        // Read existing content
        std::ostringstream ss;
        ss << inFile.rdbuf();
        std::string content = ss.str();
        inFile.close();

        // Trim trailing whitespace
        auto rtrim = [&](std::string &s) {
            while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
        };
        rtrim(content);

        if (content.empty()) {
            // overwrite
            std::ofstream outFile(filename, std::ios::out);
            outFile << "[" << entry.str() << "]\n";
            outFile.close();
            return;
        }

        // If file starts with '[' and ends with ']', insert into array
        if (content.front() == '[') {
            // find last closing bracket
            size_t pos = content.find_last_of(']');
            if (pos == std::string::npos) {
                // malformed, overwrite
                std::ofstream outFile(filename, std::ios::out);
                outFile << "[" << entry.str() << "]\n";
                outFile.close();
                return;
            }

            // Check if array is empty (e.g. "[]" or "[   ]")
            bool onlyWhitespace = true;
            for (size_t i = 1; i < pos; ++i) {
                if (!isspace((unsigned char)content[i])) { onlyWhitespace = false; break; }
            }

            std::ofstream outFile(filename, std::ios::out | std::ios::trunc);
            if (onlyWhitespace) {
                outFile << "[" << entry.str() << "]\n";
            } else {
                // Insert comma+entry before closing bracket
                std::string head = content.substr(0, pos);
                std::string tail = content.substr(pos); // includes ]
                // If head ends with whitespace, remove to keep neat formatting
                while (!head.empty() && isspace((unsigned char)head.back())) head.pop_back();
                outFile << head << "," << entry.str() << tail << "\n";
            }
            outFile.close();
            return;
        }

        // Otherwise file isn't an array — overwrite with a proper array
        std::ofstream outFile(filename, std::ios::out | std::ios::trunc);
        outFile << "[" << entry.str() << "]\n";
        outFile.close();
    }

    /************************ Landing Page ************************/
    void ShowLandingPage()
    {
        if (panel) panel->Destroy();
        panel = new wxPanel(this);
        panel->SetBackgroundColour(*wxLIGHT_GREY);
        panel->SetDoubleBuffered(true);

        winWidth = 800;
        winHeight = 600;
        SetClientSize(winWidth, winHeight);

        // Name label
        nameLabel = new wxStaticText(panel, wxID_ANY, "Enter your name:", wxPoint(300, 200), wxSize(200, 30));
        nameLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        // Name input
        nameInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 230), wxSize(200, 30));

        // Level selection
        wxArrayString levels;
        levels.Add("Level 1");
        levels.Add("Level 2");
        levels.Add("Level 3");
        levelChoice = new wxChoice(panel, wxID_ANY, wxPoint(300, 270), wxSize(200, 30), levels);
        levelChoice->SetSelection(0);

        // Start button
        wxButton* startButton = new wxButton(panel, wxID_ANY, "Start Game", wxPoint(300, 320), wxSize(200, 40));
        startButton->Bind(wxEVT_BUTTON, &MyFrame::OnStartClicked, this);
        landingButtons.push_back(startButton);

        // Help button
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
        playerNameGlobal = nameInput->GetValue();
        int selectedLevel = levelChoice->GetSelection();  // 0, 1, or 2

        if (playerNameGlobal.IsEmpty())
        {
            wxMessageBox("Please enter your name!", "Info", wxOK | wxICON_INFORMATION);
            return;
        }

        // Save selected level
        selectedLevelGlobal = selectedLevel;

        // Destroy landing UI
        if (nameLabel) { nameLabel->Destroy(); nameLabel = nullptr; }
        if (nameInput) { nameInput->Destroy(); nameInput = nullptr; }
        if (levelChoice) { levelChoice->Destroy(); levelChoice = nullptr; }
        for (auto b : landingButtons) if (b) b->Destroy();
        landingButtons.clear();

        StartCountdown();
    }

    /************************ Countdown ************************/
    void StartCountdown()
    {
        // Ensure any previous countdown is stopped
        if (m_countdownTimer.IsRunning())
            m_countdownTimer.Stop();

        countdownValue = 3;

        // Place countdown label at upper-right (leave margin)
        int margin = 10;
        int labelW = 180, labelH = 28;
        countdownLabel = new wxStaticText(panel, wxID_ANY,
                                          wxString::Format("Starting in %d...", countdownValue),
                                          wxPoint(winWidth - labelW - margin, margin),
                                          wxSize(labelW, labelH), wxALIGN_RIGHT);
        countdownLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        // Disable panel so nothing can be clicked
        panel->Disable();

        // Bind timer and start
        Bind(wxEVT_TIMER, &MyFrame::OnCountdownTick, this);
        m_countdownTimer.Start(1000); // 1 second ticks
    }

    void OnCountdownTick(wxTimerEvent&)
    {
        countdownValue--;

        if (countdownValue > 0)
        {
            if (countdownLabel && countdownLabel->IsShown())
                countdownLabel->SetLabel(wxString::Format("Starting in %d...", countdownValue));
        }
        else
        {
            if (m_countdownTimer.IsRunning()) m_countdownTimer.Stop();
            if (countdownLabel) { countdownLabel->Destroy(); countdownLabel = nullptr; }

            panel->Enable();
            timingStarted = false;
            startTimeMs = 0;
            endTimeMs = 0;

            // Load selected level
            switch (selectedLevelGlobal)
            {
                case 0: LoadLevel1Screen(0); break;
                case 1: LoadLevel2Screen(0); break;
                case 2: LoadLevel3Screen(0); break;
            }
        }
    }


    /************************ Level 1 multi-screen ************************/
    void ClearLevelButtons()
    {
        for (auto b : levelButtons) if (b) b->Destroy();
        levelButtons.clear();
    }

    void LoadLevel1Screen(int screen)
    {
        levelScreen = screen;

        // Clear previously created level buttons
        ClearLevelButtons();

        // Decide background image (if you provide images, they will be drawn)
        wxString imageFile;
        if (screen == 0) imageFile = "page_start.png";
        else if (screen == 1) imageFile = "search_criteria_page.png";
        // Change this line once the image is fixed
        else if (screen == 2) imageFile = "plan_select.png";
        else {
            wxMessageBox("Level 1 complete!", "Info", wxOK | wxICON_INFORMATION);
            return;
        }

        // Try load background (non-fatal)
        if (!bgImage.LoadFile(imageFile)) {
            // If failed to load, just keep plain background
            // wxMessageBox("Failed to load " + imageFile, "Error", wxOK | wxICON_ERROR);
        } else {
            winWidth = bgImage.GetWidth();
            winHeight = bgImage.GetHeight();
            SetClientSize(winWidth, winHeight);
        }

        // Example button bitmap size (we'll simply use standard buttons)
        // Screen-specific buttons:
        if (screen == 0)
        {
            // First button for screen 0
            wxButton* nextBtn = new wxButton(panel, wxID_ANY, "Submit", wxPoint(30,240), wxSize(150,50), wxNO_BORDER);
            nextBtn->SetBackgroundColour(wxColour(200,200,200));
            nextBtn->SetToolTip("Next");
            // Bind with timing logic: first click starts the timer (if not started)
            nextBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
            {
                if (!timingStarted) {
                    timingStarted = true;
                    startTimeMs = wxGetLocalTimeMillis();
                }
                LoadLevel1Screen(levelScreen + 1);
            });
            levelButtons.push_back(nextBtn);
        }
        else if (screen == 1)
        {
            // Button for screen 1
            // Position adjusted to be inside the window
            wxButton* nextBtn1 = new wxButton(panel, wxID_ANY, "Add Plan", wxPoint(1200,240), wxSize(150,50), wxNO_BORDER);
            nextBtn1->SetBackgroundColour(wxColour(200,200,200));
            nextBtn1->SetToolTip("Next Screen 2");
            nextBtn1->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
            {
                if (!timingStarted) {
                    timingStarted = true;
                    startTimeMs = wxGetLocalTimeMillis();
                }
                LoadLevel1Screen(levelScreen + 1);
            });
            levelButtons.push_back(nextBtn1);
        }
        
        else if (screen == 2)
        {
            // Final button - stop timer and show results
            // Place near bottom-right but inside window
            int px = std::max(20, winWidth - 200);
            int py = std::max(20, winHeight - 100);
            wxButton* finalBtn = new wxButton(panel, wxID_ANY, "Submit", wxPoint(px, py), wxSize(150,50), wxNO_BORDER);
            finalBtn->SetBackgroundColour(wxColour(0, 200, 0));
            finalBtn->SetToolTip("Finish Level 1");
            finalBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
            {
                // If timing never started (user navigated without clicking a level button), start time at earliest possible
                if (!timingStarted) {
                    startTimeMs = wxGetLocalTimeMillis();
                }
                endTimeMs = wxGetLocalTimeMillis();

                long elapsedMs = (endTimeMs - startTimeMs).ToLong();
                if (elapsedMs < 0) elapsedMs = 0; // safety

                ShowResultsScreen(elapsedMs);
            });
            levelButtons.push_back(finalBtn);
        }

        panel->Refresh();
    }

    //************************ Level 2 screen ************************/
    void LoadLevel2Screen(int screen)
    {
        levelScreen = screen;

        // Clear previously created level buttons
        ClearLevelButtons();

        // Decide background image (if you provide images, they will be drawn)
        wxString imageFile;
        if (screen == 0) imageFile = "page_start.png";
        else if (screen == 1) imageFile = "plan_select.png";
        // Change this line once the image is fixed
        else if (screen == 2) imageFile = "plan_select.png";
        else {
            wxMessageBox("Level 1 complete!", "Info", wxOK | wxICON_INFORMATION);
            return;
        }

        // Try load background (non-fatal)
        if (!bgImage.LoadFile(imageFile)) {
            // If failed to load, just keep plain background
            // wxMessageBox("Failed to load " + imageFile, "Error", wxOK | wxICON_ERROR);
        } else {
            winWidth = bgImage.GetWidth();
            winHeight = bgImage.GetHeight();
            SetClientSize(winWidth, winHeight);
        }

        // Example button bitmap size (we'll simply use standard buttons)
        // Screen-specific buttons:
        if (screen == 0)
        {
            // First button for screen 0
            wxButton* nextBtn = new wxButton(panel, wxID_ANY, "Submit", wxPoint(30,240), wxSize(150,50), wxNO_BORDER);
            nextBtn->SetBackgroundColour(wxColour(200,200,200));
            nextBtn->SetToolTip("Next");
            // Bind with timing logic: first click starts the timer (if not started)
            nextBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
            {
                if (!timingStarted) {
                    timingStarted = true;
                    startTimeMs = wxGetLocalTimeMillis();
                }
                LoadLevel2Screen(levelScreen + 1);
            });
            levelButtons.push_back(nextBtn);
        }
        else if (screen == 1)
        {
            // Button for screen 1
            // Position adjusted to be inside the window
            wxButton* nextBtn1 = new wxButton(panel, wxID_ANY, "Add Plan", wxPoint(1200,240), wxSize(150,50), wxNO_BORDER);
            nextBtn1->SetBackgroundColour(wxColour(200,200,200));
            nextBtn1->SetToolTip("Next Screen 2");
            nextBtn1->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
            {
                if (!timingStarted) {
                    timingStarted = true;
                    startTimeMs = wxGetLocalTimeMillis();
                }
                LoadLevel2Screen(levelScreen + 1);
            });
            levelButtons.push_back(nextBtn1);
        }
        
        else if (screen == 2)
        {
            // Final button - stop timer and show results
            // Place near bottom-right but inside window
            int px = std::max(20, winWidth - 200);
            int py = std::max(20, winHeight - 100);
            wxButton* finalBtn = new wxButton(panel, wxID_ANY, "Submit", wxPoint(px, py), wxSize(150,50), wxNO_BORDER);
            finalBtn->SetBackgroundColour(wxColour(0, 200, 0));
            finalBtn->SetToolTip("Finish Level 1");
            finalBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
            {
                // If timing never started (user navigated without clicking a level button), start time at earliest possible
                if (!timingStarted) {
                    startTimeMs = wxGetLocalTimeMillis();
                }
                endTimeMs = wxGetLocalTimeMillis();

                long elapsedMs = (endTimeMs - startTimeMs).ToLong();
                if (elapsedMs < 0) elapsedMs = 0; // safety

                ShowResultsScreen(elapsedMs);
            });
            levelButtons.push_back(finalBtn);
        }

        panel->Refresh();
    }

    //************************ Level 3 screen ************************/
    void LoadLevel3Screen(int screen)
    {
        levelScreen = screen;

        // Clear previously created level buttons
        ClearLevelButtons();

        // Decide background image (if you provide images, they will be drawn)
        wxString imageFile;
        if (screen == 0) imageFile = "page_start.png";
        else if (screen == 1) imageFile = "plan_select.png";
        // Change this line once the image is fixed
        else if (screen == 2) imageFile = "submit.png";
        else {
            wxMessageBox("Level 1 complete!", "Info", wxOK | wxICON_INFORMATION);
            return;
        }

        // Try load background (non-fatal)
        if (!bgImage.LoadFile(imageFile)) {
            // If failed to load, just keep plain background
            // wxMessageBox("Failed to load " + imageFile, "Error", wxOK | wxICON_ERROR);
        } else {
            winWidth = bgImage.GetWidth();
            winHeight = bgImage.GetHeight();
            SetClientSize(winWidth, winHeight);
        }

        // Example button bitmap size (we'll simply use standard buttons)
        // Screen-specific buttons:
        if (screen == 0)
        {
            // First button for screen 0
            wxButton* nextBtn = new wxButton(panel, wxID_ANY, "Submit", wxPoint(30,240), wxSize(150,50), wxNO_BORDER);
            nextBtn->SetBackgroundColour(wxColour(200,200,200));
            nextBtn->SetToolTip("Next");
            // Bind with timing logic: first click starts the timer (if not started)
            nextBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
            {
                if (!timingStarted) {
                    timingStarted = true;
                    startTimeMs = wxGetLocalTimeMillis();
                }
                LoadLevel3Screen(levelScreen + 1);
            });
            levelButtons.push_back(nextBtn);
        }
        else if (screen == 1)
        {
            // Button for screen 1
            // Position adjusted to be inside the window
            wxButton* nextBtn1 = new wxButton(panel, wxID_ANY, "Add Plan", wxPoint(1200,240), wxSize(150,50), wxNO_BORDER);
            nextBtn1->SetBackgroundColour(wxColour(200,200,200));
            nextBtn1->SetToolTip("Next Screen 2");
            nextBtn1->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
            {
                if (!timingStarted) {
                    timingStarted = true;
                    startTimeMs = wxGetLocalTimeMillis();
                }
                LoadLevel3Screen(levelScreen + 1);
            });
            levelButtons.push_back(nextBtn1);
        }
        
        else if (screen == 2)
        {
            // Final button - stop timer and show results
            // Place near bottom-right but inside window
            int px = std::max(20, winWidth - 200);
            int py = std::max(20, winHeight - 100);
            wxButton* finalBtn = new wxButton(panel, wxID_ANY, "Submit", wxPoint(px, py), wxSize(150,50), wxNO_BORDER);
            finalBtn->SetBackgroundColour(wxColour(0, 200, 0));
            finalBtn->SetToolTip("Finish Level 1");
            finalBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
            {
                // If timing never started (user navigated without clicking a level button), start time at earliest possible
                if (!timingStarted) {
                    startTimeMs = wxGetLocalTimeMillis();
                }
                endTimeMs = wxGetLocalTimeMillis();

                long elapsedMs = (endTimeMs - startTimeMs).ToLong();
                if (elapsedMs < 0) elapsedMs = 0; // safety

                ShowResultsScreen(elapsedMs);
            });
            levelButtons.push_back(finalBtn);
        }

        panel->Refresh();
    }

    /************************ Paint handler (draw bg) ************************/
    void OnPaint(wxPaintEvent&)
    {
        wxPaintDC dc(panel);
        dc.Clear();

        if (bgImage.IsOk())
            dc.DrawBitmap(wxBitmap(bgImage), 0, 0, false);
    }
    private:
    // ... existing private members ...

    void ShowLeaderboard()
    {
        std::ifstream inFile("leaderboard.json");
        if (!inFile.good()) return; // nothing to show

        std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        wxString display;
        display << "Leaderboard:\n\n";

        size_t pos = 0;
        while ((pos = content.find("{", pos)) != std::string::npos)
        {
            size_t end = content.find("}", pos);
            if (end == std::string::npos) break;
            std::string entry = content.substr(pos+1, end-pos-1);

            std::string nameStr = "unknown";
            std::string timeStr = "0";

            // simple parsing for "name":"..." and "time":...
            size_t n1 = entry.find("\"name\":\"");
            if (n1 != std::string::npos) {
                n1 += 8;
                size_t n2 = entry.find("\"", n1);
                if (n2 != std::string::npos) nameStr = entry.substr(n1, n2-n1);
            }
            size_t t1 = entry.find("\"time\":");
            if (t1 != std::string::npos) {
                t1 += 7;
                size_t t2 = entry.find(",", t1);
                if (t2 == std::string::npos) t2 = entry.length();
                timeStr = entry.substr(t1, t2-t1);
            }

            display << wxString::Format("%s : %s sec\n", nameStr, timeStr);
            pos = end + 1;
        }

        wxTextCtrl* leaderboardCtrl = new wxTextCtrl(panel, wxID_ANY, display,
                                                     wxPoint(50, 220), wxSize(500, 150),
                                                     wxTE_MULTILINE | wxTE_READONLY);
    }


    /************************ Results screen ************************/
    void ShowResultsScreen(long elapsedMs)
    {
        ClearLevelButtons();
        if (panel) panel->Destroy();
        panel = new wxPanel(this);
        panel->SetBackgroundColour(*wxWHITE);

        SetClientSize(600, 400);

        double seconds = elapsedMs / 1000.0;

        wxString msg;
        msg.Printf("You finished in %.3f seconds!", seconds);

        wxStaticText* resultText = new wxStaticText(panel, wxID_ANY, msg, wxPoint(50, 120), wxSize(500, 40), wxALIGN_CENTRE_HORIZONTAL);
        resultText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        wxStaticText* savedText = new wxStaticText(panel, wxID_ANY, "Saving your result...", wxPoint(50, 170), wxSize(500, 30), wxALIGN_CENTRE_HORIZONTAL);

        // Save to JSON
        SaveResultToJson(playerNameGlobal, seconds);

        savedText->SetLabel("Your score has been saved to leaderboard.json");

        // A button to return to landing page
        wxButton* back = new wxButton(panel, wxID_ANY, "Back to Menu", wxPoint(220, 260), wxSize(160, 40));
        back->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
            // Reset and show landing again
            if (panel) panel->Destroy();
            bgImage = wxImage(); // clear
            ShowLandingPage();
        });

        // Save to JSON
        // SaveResultToJson(playerNameGlobal, seconds);

        // Add this line to show leaderboard
        ShowLeaderboard();


        panel->Refresh();
    }
};

/****************** MAIN APP ******************/
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
