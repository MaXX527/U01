#include "MyThread.h"
//#include "Main.h"
//#include <thread>
//#include <wx/wx.h>

/*void* MyThread::Entry()
{
    std::list<Magick::Image> imageList;
    //Magick::Image image;
    //char percent[10] = "";
    try
    {
        //int n = 0; "label:@" + 
        Magick::readImages(&imageList, m_param.filename);
        for (auto img = imageList.begin(); img != imageList.end(); img++)
        {
            img->compressType(m_param.compressType);
            img->depth(m_param.depth);
            if(m_param.autoGamma)
                img->autoGamma();
            if(m_param.autoLevel)
                img->autoLevel();
            if(m_param.normalize)
                img->normalize();
            if(m_param.strip)
                img->strip();
        }
        //boost::filesystem::path path = boost::filesystem::path(m_param->filename.ToStdString());
        //std::string newfilename = path.parent_path().append(path.stem()).concat("_compress").concat(path.extension()).string();
        Magick::writeImages(imageList.begin(), imageList.end(), m_param.newfilename);
        //grid->SetCellValue(i, 3, GetFileSize(boost::filesystem::path(newfilename)));
    }
    catch (Magick::Error err)
    {
        wxMessageBox("Magick::Error: " + wxString(err.what()));
        // Какой-то обработчик ошибок
        this->Exit();
    }
    imageList.clear();

    wxThreadEvent *event = new wxThreadEvent(wxEVT_THREAD);
    event->SetPayload<TIFparam>(m_param);
    //wxGetApp().QueueEvent(event);
    wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event);

    return nullptr;
}
*/

MyThread::MyThread(TIFparam param, int type)
{
    m_param = param;
    m_type = type;

    //m_inst = NULL;

    //gsapi_new_instance(&m_inst, nullptr);
    //gsapi_set_arg_encoding(m_inst, GS_ARG_ENCODING_LOCAL);  //GS_ARG_ENCODING_UTF8
}

MyThread::~MyThread()
{
    //gsapi_delete_instance(m_inst);
}

void* MyThread::Entry()
{
    if (0 == m_type)
    {
        FreeImage_Initialise();

        FIMULTIBITMAP* fib_big = FreeImage_OpenMultiBitmap(FREE_IMAGE_FORMAT::FIF_TIFF, m_param.filename.c_str(), false, true);
        FIMULTIBITMAP* fib_big_compress = FreeImage_OpenMultiBitmap(FREE_IMAGE_FORMAT::FIF_TIFF, m_param.newfilename.c_str(), true, false);
        int countPages = FreeImage_GetPageCount(fib_big);
        FIBITMAP* fib_small;

        for (int page = 0; page < countPages; page++)
        {
            fib_small = FreeImage_LockPage(fib_big, page);
            if (fib_small)
            {
                //if (FREE_IMAGE_COLOR_TYPE::FIC_PALETTE == FreeImage_GetColorType(fib_small))
                //    FreeImage_Invert(fib_small);

                if (m_param.rescale < 1.0f)
                    fib_small = FreeImage_Rescale(fib_small, FreeImage_GetWidth(fib_small) * m_param.rescale, FreeImage_GetHeight(fib_small) * m_param.rescale);
                switch (m_param.depth)
                {
                case 1:
                    fib_small = FreeImage_Dither(fib_small, FREE_IMAGE_DITHER::FID_FS);
                    break;
                case 4:
                    fib_small = FreeImage_ConvertTo4Bits(fib_small);
                    break;
                case 8:
                    fib_small = FreeImage_ConvertToGreyscale(fib_small);
                    break;
                }

                FreeImage_AppendPage(fib_big_compress, fib_small);
                FreeImage_UnlockPage(fib_big, fib_small, false);

                FreeImage_Unload(fib_small);
                //FreeImage_Unload(fib_small_color);
            }
        }
        FreeImage_CloseMultiBitmap(fib_big_compress); // , /*TIFF_CCITTFAX4*/ /*TIFF_LZW*/);
        FreeImage_CloseMultiBitmap(fib_big);

        FreeImage_DeInitialise();

        wxThreadEvent* event = new wxThreadEvent(wxEVT_THREAD);
        event->SetPayload<TIFparam>(m_param);
        wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event);
    }
    if (m_type == 1)
    {
        /*void* minst = NULL;

        gsapi_new_instance(&minst, nullptr);
        gsapi_set_arg_encoding(minst, GS_ARG_ENCODING_LOCAL);  //GS_ARG_ENCODING_UTF8

        int gsargc = 0;
        char* gsargv[30];

        gsargv[gsargc++] = (char*)"";
        gsargv[gsargc++] = (char*)"-sDEVICE=pdfwrite";
        gsargv[gsargc++] = (char*)"-dCompatibilityLevel=1.4";
        gsargv[gsargc++] = (char*)"-dNOPAUSE";
        gsargv[gsargc++] = (char*)"-dQUIET";
        gsargv[gsargc++] = (char*)"-dBATCH";
        gsargv[gsargc++] = (char*)"-dSAFER";
        gsargv[gsargc++] = (char*)"-dOverprint=/simulate";
        gsargv[gsargc++] = (char*)"-dColorImageDownsampleType=/Bicubic";
        gsargv[gsargc++] = (char*)"-dColorImageResolution=100";
        gsargv[gsargc++] = (char*)"-dGrayImageDownsampleType=/Bicubic";
        gsargv[gsargc++] = (char*)"-dGrayImageResolution=100";
        gsargv[gsargc++] = (char*)"-dMonoImageDownsampleType=/Bicubic";
        gsargv[gsargc++] = (char*)"-dMonoImageResolution=100";
        gsargv[gsargc++] = (char*)"-dPDFSETTINGS=/ebook";
        //gsargv[gsargc++] = (char*)"-sColorConversionStrategy=Gray";
        //gsargv[gsargc++] = (char*)"-dProcessColorModel=/DeviceGray";
        gsargv[gsargc++] = (char*)"-dFIXEDMEDIA";
        gsargv[gsargc++] = (char*)"-sDEFAULTPAPERSIZE=a4";
        gsargv[gsargc++] = (char*)(("-sOutputFile=" + m_param.newfilename).c_str());
        //gsargv[gsargc++] = (char*)buf;
        //gsargv[gsargc++] = (char*)"-sOutputFile=C:\\Work\\069705-два_compress.pdf";
        gsargv[gsargc++] = (char*)(m_param.filename.c_str());

        //wxMessageBox(gsargv[gsargc - 2]);
        //wxMessageBox(gsargv[gsargc - 1]);

        //gsapi_set_stdio(minst, MyThread::gsdll_stdin, MyThread::gsdll_stdout, MyThread::gsdll_stderr);

        int exit_code;
        exit_code = gsapi_init_with_args(minst, gsargc, gsargv);
        m_param.exit_code = exit_code;
        gsapi_exit(minst);
        gsapi_delete_instance(minst);
        */

        wxString gsarg = wxString::Format(" -sDEVICE=pdfwrite " \
            "-dCompatibilityLevel=1.4 " \
            "-dNOPAUSE " \
            "-dQUIET " \
            "-dBATCH " \
            "-dSAFER " \
            "-dOverprint=/simulate " \
            "-dColorImageDownsampleType=/Bicubic " \
            "-dColorImageResolution=%d " \
            "-dGrayImageDownsampleType=/Bicubic " \
            "-dGrayImageResolution=%d " \
            "-dMonoImageDownsampleType=/Bicubic " \
            "-dMonoImageResolution=%d " \
            "-dPDFSETTINGS=/ebook " \
            "-dFIXEDMEDIA " \
            "-sDEFAULTPAPERSIZE=a4 " \
            "%s" \
            "-sOutputFile=\"%s\" \"%s\"", \
            m_param.resolution, m_param.resolution, m_param.resolution, m_param.convertToGray?"-dProcessColorModel=/DeviceGray -sColorConversionStrategy=Gray":"", m_param.newfilename, m_param.filename);

        const char* gswin64c = "C:\\Program Files\\gs\\gs10.01.1\\bin\\gswin64c.exe";
        char gsWithArgs[MAXLEN];
        strcpy_s(gsWithArgs, gswin64c);
        strcat_s(gsWithArgs, gsarg.ToStdString().c_str());

        //MyProcess* m_proc = new MyProcess(gsWithArgs, m_param); //MyProcess::Open(gsWithArgs);
        //MyProcess m_proc;
        if (wxExecute(gsWithArgs, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE, &m_proc))
        {
            while (m_proc.Exists(m_proc.GetPid())) { Sleep(100); }
            //m_proc.CallAfter(&MyThread::OnTerm);
            wxThreadEvent* event = new wxThreadEvent(wxEVT_THREAD);
            event->SetPayload<TIFparam>(m_param);
            wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event);
        }

    }
    return nullptr;
}

/* stdio functions */
int GSDLLCALL MyThread::gsdll_stdin(void* instance, char* buf, int len)
{
    int ch;
    int count = 0;
    while (count < len) {
        ch = fgetc(stdin);
        if (ch == EOF)
            return 0;
        *buf++ = ch;
        count++;
        if (ch == '\n')
            break;
    }
    return count;
}

int GSDLLCALL MyThread::gsdll_stdout(void* instance, const char* str, int len)
{
    //fwrite(str, 1, len, stdout);
    //fflush(stdout);
    //m_textCtrlErr->AppendText(str);
    //m_textCtrlErr->AppendText("\r\n");
    wxMessageBox(str);
    return len;
}

int GSDLLCALL MyThread::gsdll_stderr(void* instance, const char* str, int len)
{
    //fwrite(str, 1, len, stderr);
    //fflush(stderr);
    //m_textCtrlErr->AppendText(str);
    //m_textCtrlErr->AppendText("\r\n");
    wxMessageBox(str);
    return len;
}

void MyThread::OnTerm()
{
    wxMessageBox("OnTerm");
    wxThreadEvent* event = new wxThreadEvent(wxEVT_THREAD);
    //m_param.exit_code = status;
    event->SetPayload<TIFparam>(m_param);
    wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event);
}
