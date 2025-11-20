// HelloWorldApp.cpp : Defines the entry point for the application.
#include <wx/wx.h>
#include <iostream>
// Used to handle touppercase conversion
#include <cctype>

using namespace std;

// Main application frame
class MyFrame : public wxFrame
{
public:
    // Constructor to set up the GUI
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "String Operations", wxDefaultPosition, wxSize(360, 360))
    {
        wxPanel* panel = new wxPanel(this);

        // Create controls (wxTE_PROCESS_ENTER allows Enter key to trigger event)
        // Input text box
        inputBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1), wxTE_PROCESS_ENTER);
        // Create buttons for each operation
        // Button to double characters
        wxButton* doubleButton = new wxButton(panel, wxID_ANY, "Double", wxDefaultPosition, wxSize(70, 30));
        // Button to censor vowels
        wxButton* censorButton = new wxButton(panel, wxID_ANY, "Censor Vowels", wxDefaultPosition, wxSize(100, 30));
        // Button to mock string
        wxButton* mockButton = new wxButton(panel, wxID_ANY, "Mock", wxDefaultPosition, wxSize(70, 30));
        // Button to exit application
        wxButton* exitButton = new wxButton(panel, wxID_ANY, "Exit", wxDefaultPosition, wxSize(70, 30));

        // Static text to display output
        outputText = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));

        // Layout controls vertically
        // Create a vertical box sizer to arrange the controls
        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
        vbox->Add(inputBox, 0, wxALL | wxEXPAND, 10);
        vbox->Add(doubleButton, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);
        vbox->Add(censorButton, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);
        vbox->Add(mockButton, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);
        vbox->Add(exitButton, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);
        vbox->Add(outputText, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
        panel->SetSizer(vbox);

        // Bind events (Enter or Button)
        // Bind the Enter key and button clicks to their respective handlers
        inputBox->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnDouble, this);
        doubleButton->Bind(wxEVT_BUTTON, &MyFrame::OnDouble, this);
        censorButton->Bind(wxEVT_BUTTON, &MyFrame::OnCensor, this);
        mockButton->Bind(wxEVT_BUTTON, &MyFrame::OnMock, this);
        exitButton->Bind(wxEVT_BUTTON, &MyFrame::OnExit, this);
    }

private:
    // Input text box
    wxTextCtrl* inputBox;
    // Output static text
    wxStaticText* outputText;

    // String operation functions
    // Double each character in the string
    string double_word(string word){
        // This function takes a string and returns a new string with each character doubled.
        // For example, "hello" becomes "hheelllloo".
        string output_word;

        // Loop through each character in the input word and add it twice to the output word
        for (int i = 0; i < word.length(); i++){
            string c = string(1, word[i]);
            output_word += c + c;
        }
        return output_word;
        }

    // Censor vowels in the string
    string censor_vowels(string word){
        // This function takes a string and returns a new string with all vowels replaced by '*'.
        // For example, "hello" becomes "h*ll*".
        string output_word;

        // Loop through each character in the input word and check if it's a vowel and then replace it with a '*'
        for (int i = 0; i < word.length(); i++){
            string c = string(1, word[i]);
            if (c == "a" || c == "e" || c == "i" || c == "o" || c == "u" ||
                c == "A" || c == "E" || c == "I" || c == "O" || c == "U"){
                output_word += "*";
            } else {
                output_word += c;
            }
        }
        return output_word;
    }

    // Mock string by capitalizing characters at even indices
    string mock(string word){
        // This function takes a string and returns a new string with characters at even indices
        // converted to uppercase. For example, "hello" becomes "hElLo".
        string output_word;
        string c;

        // Loop through each character in the input word and convert characters at even indices to uppercase
        for (int i = 0; i < word.length(); i++){
            // Check if the index + 1 is even (to account for 0-based indexing)
            // We don't want the first letter to be capitalized
            if ((i+1) % 2 == 0){
                char t = toupper(word[i]);
                c = string(1, t);
            }
            else {
                c = string(1, word[i]);
            }
            output_word += c;
        }
        return output_word;
    }

    // Event handlers for button clicks and Enter key
    void OnDouble(wxCommandEvent& WXUNUSED(event))
    {
        // Get input, process it, and display output
        wxString input = inputBox->GetValue();
        // Call the double_word function
        input = double_word(input.ToStdString());
        // Set the processed string to output text
        outputText->SetLabel(input);
    }
    // Event handler for censoring vowels
    void OnCensor(wxCommandEvent& WXUNUSED(event))
    {
        wxString input = inputBox->GetValue();
        input = censor_vowels(input.ToStdString());
        outputText->SetLabel(input);
    }

    // Event handler for mocking string
    void OnMock(wxCommandEvent& WXUNUSED(event))
    {
        wxString input = inputBox->GetValue();
        input = mock(input.ToStdString());
        outputText->SetLabel(input);
    }
    // Event handler for exiting the application
    void OnExit(wxCommandEvent& WXUNUSED(event))
    {
        Close(true);
    }
};

// Main application class
class MyApp : public wxApp
{
public:
    // Override the OnInit method
    bool OnInit() override
    {
        MyFrame* frame = new MyFrame();
        frame->Show();
        return true;
    }
};

// Initialize the application
wxIMPLEMENT_APP(MyApp);
