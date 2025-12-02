// main.cpp
#include "MyFrame.h"
#include <wx/wx.h>

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
