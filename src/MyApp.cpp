#include "MyApp.h"
#include "MyFrame.h"  // Your main window

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show();
    return true;
}
