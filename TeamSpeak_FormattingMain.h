/***************************************************************
 * Name:      TeamSpeak_FormattingMain.h
 * Purpose:   Defines Application Frame
 * Author:    p512 (postmaster@thedigit.net)
 * Created:   2015-01-08
 * Copyright: p512 (thedigit.net)
 * License:
 **************************************************************/

#ifndef TEAMSPEAK_FORMATTINGMAIN_H
#define TEAMSPEAK_FORMATTINGMAIN_H

//(*Headers(TeamSpeak_FormattingDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/dialog.h>
//*)

class TeamSpeak_FormattingDialog: public wxDialog
{
    public:

        TeamSpeak_FormattingDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~TeamSpeak_FormattingDialog();

    private:

        //(*Handlers(TeamSpeak_FormattingDialog)
        void OnChoice1Select(wxCommandEvent& event);
        void OnTextCtrl1Text(wxCommandEvent& event);
        //*)

        //(*Identifiers(TeamSpeak_FormattingDialog)
        static const long ID_MODECHOICE1;
        static const long ID_INPUTTEXT1;
        static const long ID_LETTERUSAGE1;
        //*)

        //(*Declarations(TeamSpeak_FormattingDialog)
        wxTextCtrl* InputText;
        wxStaticText* LetterUsage;
        wxBoxSizer* BoxSizer2;
        wxBoxSizer* BoxSizer1;
        wxChoice* ModeChoice;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // TEAMSPEAK_FORMATTINGMAIN_H
