#pragma once
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/bmpbuttn.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>

class DialogAbout : public wxDialog
{
private:

protected:
	wxStaticBitmap* m_bmpMFC;
	wxStaticText* m_txtAbout;
	wxBitmapButton* m_btnOK;

	void OnClickOK(wxCommandEvent& event);

public:

	DialogAbout(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("О программе"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

	~DialogAbout();
};

