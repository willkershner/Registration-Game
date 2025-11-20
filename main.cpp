#include <wx/wx.h>
#include <wx/image.h>

using namespace std;

class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Background Image Example", wxDefaultPosition, wxSize(800, 500))
    {
        wxPanel* panel = new wxPanel(this);

        // Load the background image as wxImage
        if (!bgImage.LoadFile("test.png")) // replace with your image path
        {
            wxMessageBox("Failed to load background image!", "Error", wxOK | wxICON_ERROR);
        }

        // Controls
        exitButton = new wxButton(panel, wxID_ANY, "Exit", wxDefaultPosition, wxSize(70, 30));
        outputText = new wxStaticText(panel, wxID_ANY, "Hello!", wxDefaultPosition, wxSize(350, -1));

        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
        vbox->Add(outputText, 0, wxALL, 10);
        vbox->Add(exitButton, 0, wxALL, 10);
        panel->SetSizer(vbox);

        exitButton->Bind(wxEVT_BUTTON, &MyFrame::OnExit, this);

        // Bind paint event on the panel
        panel->Bind(wxEVT_PAINT, &MyFrame::OnPaint, this);
    }

private:
    wxImage bgImage;
    wxButton* exitButton;
    wxStaticText* outputText;

    void OnExit(wxCommandEvent& WXUNUSED(event))
    {
        Close(true);
    }

    void OnPaint(wxPaintEvent& WXUNUSED(event))
    {
        wxPaintDC dc(this->GetChildren()[0]); // paint on panel

        if (bgImage.IsOk())
        {
            wxSize size = this->GetClientSize();

            // Scale image to panel size
            wxBitmap bmp(bgImage.Scale(size.GetWidth(), size.GetHeight(), wxIMAGE_QUALITY_HIGH));

            // Draw scaled bitmap
            dc.DrawBitmap(bmp, 0, 0, false);
        }
    }
};

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        wxImage::AddHandler(new wxJPEGHandler);
        wxImage::AddHandler(new wxPNGHandler);
        MyFrame* frame = new MyFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
