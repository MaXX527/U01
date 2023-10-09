#include "DialogAbout.h"

DialogAbout::DialogAbout(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	m_bmpMFC = new wxStaticBitmap(this, wxID_ANY, wxBitmap("res/MFC.bmp", wxBITMAP_TYPE_BMP), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(m_bmpMFC, 0, wxALL, 5);

	m_txtAbout = new wxStaticText(this, wxID_ANY, wxT("\r\nЭто должна быть программа для сжимания TIFF и PDF\r\n(c)ОГАУ МФЦ"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	m_txtAbout->Wrap(-1);
	m_txtAbout->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString));
	bSizer4->Add(m_txtAbout, 0, wxALL, 5);


	bSizer3->Add(bSizer4, 1, wxEXPAND, 5);

	m_btnOK = new wxBitmapButton(this, wxID_ANY, /*wxNullBitmap*/ wxBitmap("res/check.bmp", wxBITMAP_TYPE_BMP), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | 0);
	m_btnOK->Bind(wxEVT_BUTTON, &DialogAbout::OnClickOK, this);
	bSizer3->Add(m_btnOK, 0, wxALIGN_CENTER | wxALL, 5);


	this->SetSizer(bSizer3);
	this->Layout();
	bSizer3->Fit(this);

	this->Centre(wxBOTH);
}

DialogAbout::~DialogAbout()
{
}

void DialogAbout::OnClickOK(wxCommandEvent& event)
{
	this->Close();
}
