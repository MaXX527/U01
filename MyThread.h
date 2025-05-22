#pragma once

#include <fstream>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/app.h>
#include <wx/process.h>
#include <wx/file.h>
#include <boost/filesystem.hpp>
#include <FreeImage.h>
#include "struct.h"
#include "Main.h"
//#include "MyProcess.h"
//#include <poppler/cpp/poppler-document.h>
//#include <poppler/cpp/poppler-page.h>
//#include <poppler/cpp/poppler-page-renderer.h>
#include <mupdf/fitz.h>

//#pragma comment(lib, "poppler-cpp.lib")
#pragma comment(lib, "libmupdf.lib")

constexpr auto ID_COMPRESSED = 100;

class MyThread : public wxThread
{
public:
    MyThread(TIFparam param, int type); // : m_param(param), m_type(type) {}
    ~MyThread();
    virtual void *Entry();

private:
    TIFparam m_param;
    int m_type; // 0 - tif, 1 - pdf
    const int tempDPI = 300;

    //void OnTerm();

    //void* m_inst;
    //MyProcess m_proc;
    wxProcess m_proc;
};
