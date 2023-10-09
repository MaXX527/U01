#pragma once

#include <wx/thread.h>
#include <wx/event.h>
#include <wx/app.h>
#include <wx/process.h>
#include <boost/filesystem.hpp>
#include <FreeImage.h>
#include "struct.h"
#include "Main.h"
//#include "MyProcess.h"

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

    static int GSDLLCALL gsdll_stdin(void* instance, char* buf, int len);
    static int GSDLLCALL gsdll_stdout(void* instance, const char* str, int len);
    static int GSDLLCALL gsdll_stderr(void* instance, const char* str, int len);
    void OnTerm();

    //void* m_inst;
    //MyProcess m_proc;
    wxProcess m_proc;
};
