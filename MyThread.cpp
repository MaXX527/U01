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
            m_param.percent = page / countPages * 100;
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_THREAD);
            evt->SetPayload<TIFparam>(m_param);
            wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(evt);
        }
        FreeImage_CloseMultiBitmap(fib_big_compress); // , /*TIFF_CCITTFAX4*/ /*TIFF_LZW*/);
        FreeImage_CloseMultiBitmap(fib_big);

        FreeImage_DeInitialise();
    }
    if (m_type == 1)
    {
        std::list<Magick::Image> imageList;
        //Magick::Image image;
        //char percent[10] = "";
        try
        {
            int n = 0;
            Magick::ReadOptions options;
            options.density(Magick::Geometry(300, 300));
            Magick::readImages(&imageList, m_param.pdffilename, options);
            for (auto img = imageList.begin(); img != imageList.end(); img++)
            {
                if(m_param.convertToGray)
                    img->colorSpace(MagickCore::ColorspaceType::GRAYColorspace);
                //img->autoGamma();
                img->autoLevel();
                img->normalize();
                //img->resize(Magick::Geometry("2548x3508"));
                //img->interlaceType(MagickCore::InterlaceType::NoInterlace);
                //img->density(100);
                //img->scale("@1500000");
                img->compressType(Magick::CompressionType::JPEGCompression);
                img->quality(m_param.quality * 10);
                img->resample(m_param.resolution);
                //wxSprintf(percent, "%0.2f %%", 100.0 * (++n) / imageList.size());
                //m_list->SetValue(percent, i, 3);
                //this->Update();
                m_param.percent = ++n / imageList.size() * 100;
                wxThreadEvent* evt = new wxThreadEvent(wxEVT_THREAD);
                evt->SetPayload<TIFparam>(m_param);
                wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(evt);
            }
            //boost::filesystem::path path = boost::filesystem::path(GetDataViewListString(i, 1).ToStdString());
            //std::string newfilename = path.parent_path().append(path.stem()).concat("_compress").concat(path.extension()).string();
            Magick::writeImages(imageList.begin(), imageList.end(), m_param.pdfnewfilename);
            //m_list->SetValue(GetFileSize(boost::filesystem::path(newfilename)), i, 3);
        }
        catch (Magick::Error err)
        {
            wxMessageBox(err.what());
        }
        imageList.clear();
    }

    wxThreadEvent* event = new wxThreadEvent(wxEVT_THREAD);
    m_param.percent = -1;
    event->SetPayload<TIFparam>(m_param);
    wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event);

    return nullptr;
}
