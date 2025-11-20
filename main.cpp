#include <wx/wx.h>
#include <wx/image.h>
#include <vector>

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& bgFile)
        : wxFrame(nullptr, wxID_ANY, "Window Matching Image Size",
                  wxDefaultPosition, wxDefaultSize,
                  wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
    {
        // Load background image
        if (!bgImage.LoadFile(bgFile))
        {
            wxMessageBox("Failed to load background image!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Set window size exactly to the image size
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

        // Add Start button (centered)
        MyButton* startButton = new MyButton(panel, emptyBmp, wxPoint(winWidth/2 - 75, winHeight/2 - 25), "Start");
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
        // Load new background image
        if (!bgImage.LoadFile("plan_select.png"))
        {
            wxMessageBox("Failed to load plan_select.png!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Set window size exactly to the new image
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
        MyButton* option1 = new MyButton(panel, emptyBmp, wxPoint(winWidth/4 - 75, winHeight/2 - 25), "Option 1");
        MyButton* option2 = new MyButton(panel, emptyBmp, wxPoint(3*winWidth/4 - 75, winHeight/2 - 25), "Option 2");
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
            // Draw image at original size (no scaling)
            dc.DrawBitmap(wxBitmap(bgImage), 0, 0, false);
        }
    }
};

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        MyFrame* frame = new MyFrame("page-start.png");
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
