// LandingPage.h
#pragma once
#include <wx/wx.h>
#include <functional>

class LandingPage : public wxPanel
{
public:
    std::function<void(const wxString&, int)> onStart;

    /**
     * Initializes the Landing Page, complete with the button for starting the game with the chosen level and inputted name
     * 
     * @param parent a pointer to the parent window, since the Landing Page is just one window in the app's frame.
     */
    LandingPage(wxWindow* parent);

private:
    wxStaticText* nameLabel = nullptr;
    wxTextCtrl* nameInput = nullptr;
    wxChoice* levelChoice = nullptr;
    std::vector<wxButton*> m_buttons;

    /**
     * An event handler for controlling the data that the player inputted on the landing page.
     * I.e., it logs the player's name and the selected level, and doesn't let them start without either of those data points.
     * 
     * @param wxCommandEvent a reference of the command event, a wxEvent-derived class that 
     */
    void OnStartClicked(wxCommandEvent&);
};
