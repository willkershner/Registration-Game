#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/stopwatch.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <iomanip> // for std::quoted

// -----------------------------
// Data structures
// -----------------------------
struct ScoreEntry {
    wxString name;
    int level;
    double seconds; // time to complete level
};

// Forward declarations
class RegistrationGameFrame;
class StartPanel;
class LevelPanel;

// -----------------------------
// StartPanel: name + level UI
// -----------------------------
class StartPanel : public wxPanel {
public:
    StartPanel(wxWindow* parent)
        : wxPanel(parent)
    {
        auto* mainSizer = new wxBoxSizer(wxVERTICAL);

        auto* title = new wxStaticText(this, wxID_ANY, "Registration Trainer");
        auto font = title->GetFont();
        font.SetPointSize(font.GetPointSize() + 6);
        font.SetWeight(wxFONTWEIGHT_BOLD);
        title->SetFont(font);

        auto* nameLabel = new wxStaticText(this, wxID_ANY, "Name:");
        m_nameCtrl = new wxTextCtrl(this, wxID_ANY);

        auto* levelLabel = new wxStaticText(this, wxID_ANY, "Level:");
        wxArrayString levels;
        levels.Add("Level 1 (Basic)");
        levels.Add("Level 2 (Buttons Move)");
        levels.Add("Level 3 (Network Issues)");
        m_levelChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, levels);
        m_levelChoice->SetSelection(0);

        auto* startBtn = new wxButton(this, wxID_ANY, "Start");

        mainSizer->Add(title, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 15);
        mainSizer->Add(nameLabel, 0, wxLEFT | wxRIGHT | wxTOP, 10);
        mainSizer->Add(m_nameCtrl, 0, wxEXPAND | wxALL, 10);
        mainSizer->Add(levelLabel, 0, wxLEFT | wxRIGHT | wxTOP, 10);
        mainSizer->Add(m_levelChoice, 0, wxEXPAND | wxALL, 10);
        mainSizer->Add(startBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 15);

        SetSizer(mainSizer);

        startBtn->Bind(wxEVT_BUTTON, &StartPanel::OnStartClicked, this);
    }

    wxString GetPlayerName() const {
        return m_nameCtrl->GetValue();
    }

    int GetSelectedLevel() const {
        // Level 1 => 1, Level 2 => 2, Level 3 => 3
        return m_levelChoice->GetSelection() + 1;
    }

    void SetPlayerName(const wxString& name) {
        m_nameCtrl->SetValue(name);
    }

private:
    void OnStartClicked(wxCommandEvent& evt);

    wxTextCtrl* m_nameCtrl{nullptr};
    wxChoice*   m_levelChoice{nullptr};
};

// -----------------------------
// LevelPanel: simulated steps
// -----------------------------
class LevelPanel : public wxPanel {
public:
    LevelPanel(wxWindow* parent, RegistrationGameFrame* frame, int level);

    void Reset();

private:
    void CreateUI();
    void OnStepButton(wxCommandEvent& evt);
    void ShuffleButtons();         // for Level 2
    bool SimulateNetworkIssue();   // for Level 3

    RegistrationGameFrame* m_frame{nullptr};
    int m_level{1};
    std::vector<wxString> m_steps;
    std::vector<wxButton*> m_buttons;
    int m_currentStep{0};
    bool m_layoutChanged{false};
    std::mt19937 m_rng;

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_TIMER = wxID_HIGHEST + 1,
};

// -----------------------------
// Main Frame
// -----------------------------
class RegistrationGameFrame : public wxFrame {
public:
    RegistrationGameFrame()
        : wxFrame(nullptr, wxID_ANY, "Registration Trainer", wxDefaultPosition, wxSize(900, 600)),
          m_timer(this, ID_TIMER)
    {
        // Menu bar
        auto* menuBar = new wxMenuBar;
        auto* fileMenu = new wxMenu;
        fileMenu->Append(wxID_EXIT, "E&xit");

        auto* viewMenu = new wxMenu;
        auto* leaderboardItem = viewMenu->Append(wxID_ANY, "&Leaderboard\tCtrl+L");

        menuBar->Append(fileMenu, "&File");
        menuBar->Append(viewMenu, "&View");
        SetMenuBar(menuBar);

        // Status bar
        CreateStatusBar();
        SetStatusText("Welcome to the Registration Trainer!");

        // Top area: timer label
        auto* topPanel = new wxPanel(this);
        auto* topSizer = new wxBoxSizer(wxHORIZONTAL);
        m_timeLabel = new wxStaticText(topPanel, wxID_ANY, "Time: 06:59:10");
        auto font = m_timeLabel->GetFont();
        font.SetPointSize(font.GetPointSize() + 2);
        font.SetWeight(wxFONTWEIGHT_BOLD);
        m_timeLabel->SetFont(font);

        topSizer->AddStretchSpacer(1);
        topSizer->Add(m_timeLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 8);
        topPanel->SetSizer(topSizer);

        // Center area: content (start panel or level panel)
        m_mainPanel = new wxPanel(this);
        m_mainSizer = new wxBoxSizer(wxVERTICAL);
        m_mainPanel->SetSizer(m_mainSizer);

        // Layout
        auto* frameSizer = new wxBoxSizer(wxVERTICAL);
        frameSizer->Add(topPanel, 0, wxEXPAND);
        frameSizer->Add(m_mainPanel, 1, wxEXPAND);
        SetSizerAndFit(frameSizer); // <--- CORRECT

        // Load leaderboard from file
        LoadLeaderboard();

        // Start screen
        ShowStartPanel();

        // Timer + menu bindings
        Bind(wxEVT_TIMER, &RegistrationGameFrame::OnTimer, this, ID_TIMER);
        Bind(wxEVT_MENU, &RegistrationGameFrame::OnExit, this, wxID_EXIT);
        Bind(wxEVT_MENU, &RegistrationGameFrame::OnShowLeaderboardMenu, this, leaderboardItem->GetId());

        m_rng.seed(std::random_device{}());
        Centre();
    }

    void ShowStartPanel() {
        m_gameRunning = false;

        // Clear old content
        if (m_currentContent) {
            m_mainSizer->Detach(m_currentContent);
            m_currentContent->Destroy();
            m_currentContent = nullptr;
        }

        auto* start = new StartPanel(m_mainPanel);
        // Pre-fill name if we have one
        if (!m_playerName.IsEmpty()) {
            start->SetPlayerName(m_playerName);
        }

        m_mainSizer->Add(start, 1, wxEXPAND | wxALL, 10);
        m_mainPanel->Layout();
        m_currentContent = start;
    }

    void StartLevel(const wxString& playerName, int level) {
        if (playerName.IsEmpty()) {
            wxMessageBox("Please enter your name before starting.", "Missing name", wxOK | wxICON_INFORMATION, this);
            return;
        }

        m_playerName = playerName;
        m_currentLevel = level;

        // Clear old content
        if (m_currentContent) {
            m_mainSizer->Detach(m_currentContent);
            m_currentContent->Destroy();
            m_currentContent = nullptr;
        }

        auto* levelPanel = new LevelPanel(m_mainPanel, this, level);
        m_mainSizer->Add(levelPanel, 1, wxEXPAND | wxALL, 10);
        m_mainPanel->Layout();
        m_currentContent = levelPanel;

        // Reset and start timers
        m_secondsSinceStart = 0;
        UpdateSimulatedTimeLabel();
        m_stopWatch.Start();
        m_gameRunning = true;
        m_timer.Start(1000); // 1 second
    }

    void OnLevelCompleted() {
        m_timer.Stop();
        m_gameRunning = false;
        long ms = m_stopWatch.Time();
        double seconds = ms / 1000.0;

        // Store score
        ScoreEntry entry;
        entry.name = m_playerName;
        entry.level = m_currentLevel;
        entry.seconds = seconds;
        m_scores.push_back(entry);
        SaveLeaderboard();

        wxString msg;
        msg.Printf("Nice work, %s!\n\nLevel %d completed in %.2f seconds.",
                   m_playerName, m_currentLevel, seconds);
        wxMessageBox(msg, "Level Complete", wxOK | wxICON_INFORMATION, this);

        ShowStartPanel();
    }

    void OnTimer(wxTimerEvent&) {
        if (!m_gameRunning) return;
        ++m_secondsSinceStart;
        UpdateSimulatedTimeLabel();
    }

    void UpdateSimulatedTimeLabel() {
        // Simulated clock:
        // Start at 06:59:10, tick every second
        int baseSeconds = 6 * 3600 + 59 * 60 + 10;
        int total = baseSeconds + m_secondsSinceStart;

        int hour = (total / 3600) % 24;
        int minute = (total / 60) % 60;
        int second = total % 60;

        wxString text;
        text.Printf("Time: %02d:%02d:%02d", hour, minute, second);
        m_timeLabel->SetLabel(text);
    }

    void ShowLeaderboardDialog() {
        if (m_scores.empty()) {
            wxMessageBox("No entries in the leaderboard yet.", "Leaderboard", wxOK | wxICON_INFORMATION, this);
            return;
        }

        // Sort by time (ascending)
        std::vector<ScoreEntry> sorted = m_scores;
        std::sort(sorted.begin(), sorted.end(),
                  [](const ScoreEntry& a, const ScoreEntry& b) {
                      return a.seconds < b.seconds;
                  });

        wxString msg;
        msg << "Name\tLevel\tTime (s)\n";
        msg << "------------------------------\n";
        for (const auto& e : sorted) {
            msg << e.name << "\t" << e.level << "\t" << wxString::Format("%.2f", e.seconds) << "\n";
        }

        wxMessageBox(msg, "Leaderboard", wxOK | wxICON_INFORMATION, this);
    }

    void LoadLeaderboard() {
        m_scores.clear();
        std::ifstream in("leaderboard.txt");
        if (!in.is_open()) return;

        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::istringstream iss(line);
            std::string name;
            int level;
            double secs;
            if (!(iss >> std::quoted(name) >> level >> secs)) {
                continue;
            }
            ScoreEntry e;
            e.name = name;
            e.level = level;
            e.seconds = secs;
            m_scores.push_back(e);
        }
    }

    void SaveLeaderboard() {
        std::ofstream out("leaderboard.txt", std::ios::trunc);
        if (!out.is_open()) return;

        for (const auto& e : m_scores) {
            // Save name quoted so spaces are preserved
            out << std::quoted(std::string(e.name.mb_str())) << " "
                << e.level << " "
                << e.seconds << "\n";
        }
    }

private:
    void OnExit(wxCommandEvent&) {
        Close(true);
    }

    void OnShowLeaderboardMenu(wxCommandEvent&) {
        ShowLeaderboardDialog();
    }

    wxPanel*    m_mainPanel{nullptr};
    wxBoxSizer* m_mainSizer{nullptr};
    wxWindow*   m_currentContent{nullptr};

    wxStaticText* m_timeLabel{nullptr};
    wxTimer       m_timer;
    wxStopWatch   m_stopWatch;
    bool          m_gameRunning{false};
    int           m_secondsSinceStart{0};

    wxString m_playerName;
    int      m_currentLevel{1};
    std::vector<ScoreEntry> m_scores;

    std::mt19937 m_rng;
};

// Implementation of StartPanel's button handler now that frame exists
void StartPanel::OnStartClicked(wxCommandEvent& evt) {
    auto* frame = dynamic_cast<RegistrationGameFrame*>(GetParent()->GetParent());
    if (frame) {
        frame->StartLevel(GetPlayerName(), GetSelectedLevel());
    }
    evt.Skip();
}

// -----------------------------
// LevelPanel implementation
// -----------------------------
wxBEGIN_EVENT_TABLE(LevelPanel, wxPanel)
    // (empty – we use Bind in code)
wxEND_EVENT_TABLE()

LevelPanel::LevelPanel(wxWindow* parent, RegistrationGameFrame* frame, int level)
    : wxPanel(parent),
      m_frame(frame),
      m_level(level)
{
    m_rng.seed(std::random_device{}());

    // A simple sequence of "BannerWeb" style steps
    m_steps = {
        "Open BannerWeb",
        "Login",
        "Student Services",
        "Registration",
        "Add/Drop Classes",
        "Enter CRNs",
        "Submit",
        "Confirm"
    };

    CreateUI();
}

void LevelPanel::CreateUI() {
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxString titleText;
    titleText.Printf("Level %d", m_level);
    auto* title = new wxStaticText(this, wxID_ANY, titleText);
    auto font = title->GetFont();
    font.SetPointSize(font.GetPointSize() + 4);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    title->SetFont(font);

    auto* subtitle = new wxStaticText(
        this, wxID_ANY,
        "Click the registration steps in the correct order as fast as possible.\n"
        "Level 1: Normal\n"
        "Level 2: Buttons move halfway through\n"
        "Level 3: Random network errors");
    subtitle->Wrap(600);

    mainSizer->Add(title, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    mainSizer->Add(subtitle, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

    // Grid of buttons
    auto* grid = new wxGridSizer(2, 4, 10, 10); // 2 rows, 4 columns (for 8 steps)
    for (size_t i = 0; i < m_steps.size(); ++i) {
        auto* btn = new wxButton(this, wxID_ANY, m_steps[i]);
        btn->Bind(wxEVT_BUTTON, &LevelPanel::OnStepButton, this);
        m_buttons.push_back(btn);
        grid->Add(btn, 1, wxEXPAND);
    }

    mainSizer->Add(grid, 1, wxEXPAND | wxALL, 20);
    SetSizer(mainSizer);
}

void LevelPanel::Reset() {
    m_currentStep = 0;
    m_layoutChanged = false;
    for (auto* b : m_buttons) {
        b->Enable(true);
    }
    Layout();
}

void LevelPanel::OnStepButton(wxCommandEvent& evt) {
    auto* btn = dynamic_cast<wxButton*>(evt.GetEventObject());
    if (!btn) {
        evt.Skip();
        return;
    }

    // Find index of button
    int idx = -1;
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i] == btn) {
            idx = static_cast<int>(i);
            break;
        }
    }
    if (idx == -1) {
        evt.Skip();
        return;
    }

    // For Level 3, simulate network issues: sometimes ignore click
    if (m_level == 3) {
        if (SimulateNetworkIssue()) {
            wxMessageBox("Network error: request did not go through.\nTry again!", "Network Issue",
                         wxOK | wxICON_WARNING, this);
            return;
        }
    }

    if (idx == m_currentStep) {
        // Correct button
        btn->Enable(false);
        m_currentStep++;

        // Level 2: Move buttons halfway through
        if (m_level == 2 && !m_layoutChanged && m_currentStep == static_cast<int>(m_steps.size() / 2)) {
            ShuffleButtons();
            m_layoutChanged = true;
        }

        if (m_currentStep == static_cast<int>(m_steps.size())) {
            // Completed level
            if (m_frame) {
                m_frame->OnLevelCompleted();
            }
        }
    } else {
        // Incorrect button
        wxMessageBox("Wrong step! Remember the correct order from BannerWeb.", "Incorrect",
                     wxOK | wxICON_ERROR, this);
    }

    evt.Skip();
}

void LevelPanel::ShuffleButtons() {
    // Randomly reorder buttons in the sizer to simulate UI change
    auto* sizer = GetSizer();
    if (!sizer) return;

    // The main sizer: [0] title, [1] subtitle, [2] grid
    if (sizer->GetItemCount() < 3) return;
    auto* gridItem = sizer->GetItem(2);
    auto* gridSizer = gridItem->GetSizer();
    if (!gridSizer) return;

    // Clear grid layout but keep buttons alive
    gridSizer->Clear(false); // false => don't delete windows

    // Create an index vector [0..7], shuffle it, and re-add buttons in new order
    std::vector<int> indices(m_buttons.size());
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        indices[i] = static_cast<int>(i);
    }
    std::shuffle(indices.begin(), indices.end(), m_rng);

    for (int idx : indices) {
        gridSizer->Add(m_buttons[idx], 1, wxEXPAND);
    }

    Layout();
}

bool LevelPanel::SimulateNetworkIssue() {
    // Around 25% chance of a network failure
    std::uniform_int_distribution<int> dist(0, 3);
    int r = dist(m_rng);
    return r == 0;
}

// -----------------------------
// wxApp
// -----------------------------
class RegistrationGameApp : public wxApp {
public:
    bool OnInit() override {
        auto* frame = new RegistrationGameFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(RegistrationGameApp);