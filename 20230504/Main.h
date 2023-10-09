#pragma once

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/spinctrl.h>
#include <wx/dataview.h>
#include <wx/config.h>
#include <wx/stdpaths.h>

#include <boost/filesystem.hpp>

//#include <Magick++.h>

#include "ghostscript/iapi.h"
#include "ghostscript/ierrors.h"
#include "ghostscript/gserrors.h"
#define GSDLLEXPORT __declspec(dllimport)

#include <Shlwapi.h>

#include "struct.h"
#include "MyThread.h"

//#define wxDEBUG_LEVEL 0

#pragma comment(lib, "wxbase32u.lib")
#pragma comment(lib, "libboost_filesystem-vc142-mt-gd-x64-1_82.lib")
//#pragma comment(lib, "CORE_RL_Magick++_.lib")
//#pragma comment(lib, "CORE_RL_MagickCore_.lib")
#pragma comment(lib, "gsdll64.lib")
#pragma comment(lib, "FreeImage.lib")

constexpr auto MAXLEN = 2000;

class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

DECLARE_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    wxDataViewListCtrl* m_list;

    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnClose(wxCloseEvent& event);

    void OnDropFiles(wxDropFilesEvent& event);
    void Check_File(const boost::filesystem::path& p);
    wxString GetFileSize(const boost::filesystem::path& filename);
    wxString PrintSize(DWORD bytes);
    void OnClickPDF(wxCommandEvent& event);

    void OnTIFCompressed(wxThreadEvent& event);
    void OnSize(wxSizeEvent& event);

    wxFlexGridSizer* m_fgTIFSizer;
    wxFlexGridSizer* m_fgPDFSizer;
    wxChoice* m_choiceTIFrescale;
    wxChoice* m_choiceTIFdepth;
    wxCheckBox* m_checkPDFgray;
    wxSpinCtrl* m_spinPDFdpi;

    wxString GetDataViewListString(int row, int col);

    wxConfig* m_config;
    void LoadConfig();
    void SaveConfig();

    void OpenFile(wxCommandEvent& event);
    void OpenDir(wxCommandEvent& event);

    void OnDeleteItem(wxKeyEvent& event);
};
