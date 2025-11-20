#include <wx/wx.h>
#include <wx/image.h>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#endif

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& bgFile)
        : wxFrame(nullptr, wxID_ANY, "Pixel-Perfect Image Window",
                  wxPoint(20, 50),  // <-- position window at left
                  wxDefaultSize,
                  wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
    {
        // Load background image
        if (!bgImage.LoadFile(bgFile))
        {
            wxMessageBox("Failed to load image!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Set window size to match image exactly
        winWidth = bgImage.GetWidth();
        winHeight = bgImage.GetHeight();
        SetSize(winWidth, winHeight);
        SetSizeHints(wxSize(winWidth, winHeight), wxSize(winWidth, winHeight));

        // Create panel
        panel = new wxPanel(this);
        panel->SetBackgroundStyle(wxBG_STYLE_PAINT);
        panel->SetDoubleBuffered(true);

        // Transparent bitmap for buttons
        wxBitmap emptyBmp(150, 50);
        {
            wxMemoryDC dc(emptyBmp);
            dc.SetBackground(*wxTRANSPARENT_BRUSH);
            dc.Clear();
            dc.SelectObject(wxNullBitmap);
        }

        // Add Start button centered
        MyButton* startButton = new MyButton(panel, emptyBmp,
                                             wxPoint(winWidth / 2 - 75, winHeight / 2 - 25),
                                             "Start");
        buttons.push_back(startButton);
        startButton->Bind(wxEVT_BUTTON, &MyFrame::OnStartClicked, this);

        // Bind paint event
        panel->Bind(wxEVT_PAINT, &MyFrame::OnPaint, this);
    }

private:
    wxPanel* panel;
    wxImage bgImage;
    std::vector<wxButton*> buttons;
    int winWidth, winHeight;

    class MyButton : public wxBitmapButton
    {
    public:
        MyButton(wxPanel* parent, const wxBitmap& bmp, const wxPoint& pos, const wxString& label)
            : wxBitmapButton(parent, wxID_ANY, bmp, pos, bmp.GetSize(), wxBORDER_NONE)
        {
            SetToolTip(label);
        }
    };

    void OnStartClicked(wxCommandEvent&)
    {
        if (!bgImage.LoadFile("plan_select.png"))
        {
            wxMessageBox("Failed to load plan_select.png!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Resize window to match new image size
        winWidth = bgImage.GetWidth();
        winHeight = bgImage.GetHeight();
        SetSize(winWidth, winHeight);
        SetSizeHints(wxSize(winWidth, winHeight), wxSize(winWidth, winHeight));

        // Remove old buttons
        for (auto btn : buttons) btn->Destroy();
        buttons.clear();

        // Transparent bitmap for buttons
        wxBitmap emptyBmp(150, 50);
        {
            wxMemoryDC dc(emptyBmp);
            dc.SetBackground(*wxTRANSPARENT_BRUSH);
            dc.Clear();
            dc.SelectObject(wxNullBitmap);
        }

        // Add new buttons relative to window size
        MyButton* option1 = new MyButton(panel, emptyBmp,
                                         wxPoint(winWidth / 4 - 75, winHeight / 2 - 25),
                                         "Option 1");
        MyButton* option2 = new MyButton(panel, emptyBmp,
                                         wxPoint(3 * winWidth / 4 - 75, winHeight / 2 - 25),
                                         "Option 2");
        buttons.push_back(option1);
        buttons.push_back(option2);

        option1->Bind(wxEVT_BUTTON, &MyFrame::OnOptionClicked, this);
        option2->Bind(wxEVT_BUTTON, &MyFrame::OnOptionClicked, this);

        panel->Layout();
        panel->Refresh();
    }

    void OnOptionClicked(wxCommandEvent&)
    {
        wxMessageBox("Option clicked!", "Info", wxOK | wxICON_INFORMATION);
    }

    void OnPaint(wxPaintEvent&)
    {
        wxPaintDC dc(panel);
        dc.Clear();

        if (bgImage.IsOk())
        {
            dc.DrawBitmap(wxBitmap(bgImage), 0, 0, false);
        }
    }
};

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
#if defined(_WIN32)
        SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        MyFrame* frame = new MyFrame("page_start.png");
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
