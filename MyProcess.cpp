#include "MyProcess.h"

void MyProcess::OnTerminate(int pid, int status)
{
    wxMessageBox("OnTerm");
    wxThreadEvent* event = new wxThreadEvent(wxEVT_THREAD);
    m_param.exit_code = status;
    event->SetPayload<TIFparam>(m_param);
    wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event);
}