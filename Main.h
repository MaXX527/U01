#pragma once

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/spinctrl.h>
#include <wx/dataview.h>
#include <wx/config.h>
#include <wx/stdpaths.h>
#include <wx/dynarray.h>
#include <wx/uiaction.h>

#include <boost/filesystem.hpp>

#include <Magick++.h>

//#include "ghostscript/iapi.h"
//#include "ghostscript/ierrors.h"
//#include "ghostscript/gserrors.h"
//#define GSDLLEXPORT __declspec(dllimport)

#include <Shlwapi.h>
//#include <stdlib.h>

#include "struct.h"
#include "MyThread.h"
#include "DialogAbout.h"
//#include <wx/config.h>
#include "resource.h"

//#define wxDEBUG_LEVEL 0

#ifdef _DEBUG
#pragma comment(lib, "wxbase32ud.lib")
#else
#pragma comment(lib, "wxbase32u.lib")
#endif
#pragma comment(lib, "libboost_filesystem-vc143-mt-gd-x64-1_87.lib")
#pragma comment(lib, "CORE_RL_Magick++_.lib")
#pragma comment(lib, "CORE_RL_MagickCore_.lib")
#pragma comment(lib, "gsdll64.lib")
#pragma comment(lib, "FreeImage.lib")
#pragma comment(lib, "Shlwapi.lib")

constexpr auto MAXLEN = 2000;

enum
{
    ID_Hello = 1,
    ID_PDF,
    ID_choiceTIFrescale,
    ID_choiceTIFdepth,
    ID_Clear,
    ID_OpenFile,
    ID_OpenDir,
    ID_SpinQuality,
    ID_MenuOpenFile,
    ID_MenuOpenDir,
    ID_PopupOpenOld,
    ID_PopupOpenNew,
    ID_PopupDeleteRow,
    ID_PopupClear
};

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
    //void OnSpinQualityUp(wxSpinEvent& event);
    //void OnSpinQualityDown(wxSpinEvent& event);
    void OnSliderChange(wxScrollEvent& event);

    wxFlexGridSizer* m_fgTIFSizer;
    wxFlexGridSizer* m_fgPDFSizer;
    wxChoice* m_choiceTIFrescale;
    wxChoice* m_choiceTIFdepth;
    wxCheckBox* m_checkPDFgray;
    wxSpinCtrl* m_spinPDFdpi;
    //wxSpinCtrl* m_spinPDFquality;
    wxSlider* m_sliderPDFquality;
    wxStaticText* m_labelPDFquality;

    wxString GetDataViewListString(int row, int col);
    void OnContextMenu(wxDataViewEvent& event);
    //void OpenOldFile(wxEvent& event);
    //void OpenNewFile(wxEvent& event);

    wxConfig* m_config;
    void LoadConfig();
    void SaveConfig();

    void OpenFile(wxCommandEvent& event);
    void OpenDir(wxCommandEvent& event);

    void OnDeleteItem(wxKeyEvent& event);

    wxHashTable m_timers;
    //wxHashTable m_threads;
    //std::list<int, MyThread*> m_threads;
    wxVector<wxThread *> m_threads;

    int m_coreCount;
    int m_threadCount;
    wxCriticalSection *m_critical;
};
