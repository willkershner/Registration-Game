// LandingPage.h
#pragma once
#include <wx/wx.h>
#include <functional>

class LandingPage : public wxPanel
{
public:
    std::function<void(const wxString&, int)> onStart;
    LandingPage(wxWindow* parent);

private:
    wxStaticText* nameLabel = nullptr;
    wxTextCtrl* nameInput = nullptr;
    wxChoice* levelChoice = nullptr;
    std::vector<wxButton*> m_buttons;

    void OnStartClicked(wxCommandEvent&);
};
