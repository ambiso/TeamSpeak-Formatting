/***************************************************************
 * Name:      TeamSpeak_FormattingApp.cpp
 * Purpose:   Code for Application Class
 * Author:    p512 (postmaster@thedigit.net)
 * Created:   2015-01-08
 * Copyright: p512 (thedigit.net)
 * License:
 **************************************************************/

#include "TeamSpeak_FormattingApp.h"

//(*AppHeaders
#include "TeamSpeak_FormattingMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(TeamSpeak_FormattingApp);

bool TeamSpeak_FormattingApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	TeamSpeak_FormattingDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
