/***************************************************************
 * Name:      TeamSpeak_FormattingMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    p512 (postmaster@thedigit.net)
 * Created:   2015-01-08
 * Copyright: p512 (thedigit.net)
 * License:
 **************************************************************/

#include "TeamSpeak_FormattingMain.h"
#include <wx/msgdlg.h>
#include <wx/clipbrd.h>
#include <iostream>
#include <fstream>
#include <map>
#include "include/modeFactory.h"
#include "include/helpers.hpp"
#include <algorithm>

const int VERSION = 1;

typedef std::map<std::string, modeFactory*> modeMap;

modeMap modes;
std::string mode = "default";

//(*InternalHeaders(TeamSpeak_FormattingDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(TeamSpeak_FormattingDialog)
const long TeamSpeak_FormattingDialog::ID_MODECHOICE1 = wxNewId();
const long TeamSpeak_FormattingDialog::ID_INPUTTEXT1 = wxNewId();
const long TeamSpeak_FormattingDialog::ID_LETTERUSAGE1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(TeamSpeak_FormattingDialog,wxDialog)
    //(*EventTable(TeamSpeak_FormattingDialog)
    //*)
END_EVENT_TABLE()

TeamSpeak_FormattingDialog::TeamSpeak_FormattingDialog(wxWindow* parent,wxWindowID id)
{
    modes.clear();
    modes["rainbow"] = new colorRainbow();
    modes["ghostly"] = new colorFadeout();
    modes["spook"] = new colorFadeInAndOut();
    modes["emergency"] = new colorEmergency();
    modes["red"] = new colorRed();

    wxArrayString choices;
    for(modeMap::iterator it = modes.begin(); it != modes.end(); ++it)
    {
        choices.Add(it->first);
    }

    modes["default"] = new colorRainbow();

    //(*Initialize(TeamSpeak_FormattingDialog)
    Create(parent, wxID_ANY, _("TeamSpeak Formatting"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    ModeChoice = new wxChoice(this, ID_MODECHOICE1, wxDefaultPosition, wxSize(138,27), 0, 0, 0, wxDefaultValidator, _T("ID_MODECHOICE1"));
    ModeChoice->SetSelection( ModeChoice->Append(_("default")) );
    ModeChoice->SetToolTip(_("Select a mode to format your text with."));
    BoxSizer2->Add(ModeChoice, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    InputText = new wxTextCtrl(this, ID_INPUTTEXT1, wxEmptyString, wxDefaultPosition, wxSize(314,27), 0, wxDefaultValidator, _T("ID_INPUTTEXT1"));
    InputText->SetFocus();
    InputText->SetToolTip(_("Enter the text you want to format here. Upon entering the formatted text will be copied to clipboard."));
    BoxSizer2->Add(InputText, 1, wxALL|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
    LetterUsage = new wxStaticText(this, ID_LETTERUSAGE1, _("length: 0"), wxDefaultPosition, wxSize(106,16), 0, _T("ID_LETTERUSAGE1"));
    BoxSizer2->Add(LetterUsage, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_MODECHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&TeamSpeak_FormattingDialog::OnChoice1Select);
    Connect(ID_INPUTTEXT1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&TeamSpeak_FormattingDialog::OnTextCtrl1Text);
    //*)

    ModeChoice ->Append(choices);
}


TeamSpeak_FormattingDialog::~TeamSpeak_FormattingDialog()
{
    //(*Destroy(TeamSpeak_FormattingDialog)
    //*)
}

void TeamSpeak_FormattingDialog::OnChoice1Select(wxCommandEvent& event)
{
    mode = ModeChoice->GetString(ModeChoice->GetSelection()).mb_str();
    mode = toLower(mode);
}

void TeamSpeak_FormattingDialog::OnTextCtrl1Text(wxCommandEvent& event)
{
    modeFactory* color = modes[mode];
    std::string formatted = std::string(InputText->GetValue().mb_str());
    if(formatted.length() > 0 && wxTheClipboard->Open())
    {
        std::string formatted = compress(reFormat(formatted, color));

        wxTheClipboard->SetData(new wxTextDataObject(formatted));
        wxTheClipboard->Close();
    }
    int length = formatted.length() + std::count(formatted.begin(), formatted.end(), '/');
    LetterUsage->SetLabel(wxString::Format(wxT("length: %i"), length));
}
