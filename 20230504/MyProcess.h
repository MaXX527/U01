#pragma once
#include <wx/process.h>
#include "MyThread.h"
#include "struct.h"

class MyProcess : public wxProcess
{
public:
    /*MyProcess(MyFrame* parent, const wxString& cmd, TIFparam param)
        : wxProcess(parent), m_cmd(cmd), m_param(param)*/
    MyProcess()
    {
    }

    MyProcess(const wxString& cmd, TIFparam param) : m_cmd(cmd), m_param(param)
    {
        //m_parent = parent;
        this->Open(m_cmd);
    }

    // instead of overriding this virtual function we might as well process the
    // event from it in the frame class - this might be more convenient in some
    // cases
    virtual void OnTerminate(int pid, int status) wxOVERRIDE;

protected:
    //MyFrame* m_parent;
    wxString m_cmd;
    TIFparam m_param;
};
