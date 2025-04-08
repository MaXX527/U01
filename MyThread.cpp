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
                if (FREE_IMAGE_COLOR_TYPE::FIC_PALETTE == FreeImage_GetColorType(fib_small))
                    FreeImage_Invert(fib_small);

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
        Magick::ReadOptions options;
        options.density(Magick::Geometry(300, 300));

        fz_context* ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
        fz_register_document_handlers(ctx);
        fz_document* doc = fz_open_document(ctx, m_param.pdffilename.c_str());
        int page_count = fz_count_pages(ctx, doc);
        //wxMessageBox(wxString::Format("%d", page_count));
        fz_matrix ctm = fz_scale(300.0/96, 300.0/96);
        for (int i = 0; i < page_count; i++)
        {
            fz_pixmap* pix = fz_new_pixmap_from_page_number(ctx, doc, i, ctm, fz_device_rgb(ctx), 0);
            
            //wxMessageBox(wxString::Format("%d x %d", pix->w, pix->h));
            const std::string pfile = wxString::Format("%s-%d.jpg", m_param.pdffilename, i).ToStdString();
            pix->xres = 300;
            pix->yres = 300;
            fz_save_pixmap_as_jpeg(ctx, pix, pfile.c_str(), 70);
            fz_drop_pixmap(ctx, pix);
            Magick::readImages(&imageList, pfile, options);
            boost::filesystem::remove(pfile);
        }
        fz_drop_document(ctx, doc);
        fz_drop_context(ctx);
        //Magick::Image image;
        //char percent[10] = "";
        //poppler::document* doc = poppler::document::load_from_file(m_param.pdffilename);
        //std::ifstream ifile(m_param.pdffilename, std::ios_base::binary);
        //ifile.seekg(0, std::ios::end);
        //auto lengthFile = ifile.tellg();
        //ifile.seekg(0, std::ios::beg);
        //poppler::byte_array* pbarr = new poppler::byte_array(static_cast<long>(lengthFile), 0);
        //ifile.read(pbarr->data(), static_cast<long>(lengthFile));
        //wxMessageBox(wxString::Format("%d", static_cast<int>(pbarr->size())));
        //poppler::document* doc = poppler::document::load_from_raw_data(pbarr->data(), static_cast<long>(lengthFile));
        //poppler::document *doc = poppler::document::load_from_file(m_param.pdffilename);
        //wxMessageBox(wxString::Format("%d", doc->pages()));
        //ifile.close();
        //if (NULL == doc)
        //{
        //   wxMessageBox(m_param.pdffilename);
        //    return nullptr;
        //}
        try
        {
            int n = 0;
            //Magick::ReadOptions options;
            //options.density(Magick::Geometry(300, 300));
            //try
            //{
                //poppler::image pimg;
                //poppler::page_renderer prend;
                for (int i = 0; i < page_count; i++)
                {
                    //const std::string pfile = wxString::Format("%s-%d.jpg", m_param.pdffilename, i).ToStdString();
                    //wxMessageBox(pfile);
                    //poppler::image pimg = prend.render_page(doc->create_page(i), 300, 300);
                    
                    //doc->save_a_copy("C:/1/copy.pdf");
                    
                    //std::vector<std::string> pformats = pimg.supported_image_formats();
                    //for (std::string f : pformats)
                    //    wxMessageBox(wxString::Format("%d", static_cast<int>(pformats.size())));
                    //wxMessageBox(wxString::Format("%d x %d",pimg.width(), pimg.height()));
                    //if(pimg.format() == poppler::image::format_argb32)
                    //    wxMessageBox("OK");
                    //pimg.save(pfile, "JPEG");
                    
                    //Magick::Blob mblob(&pimg, strlen(pimg.const_data()));
                    //Magick::Geometry mgeom(pimg.width(), pimg.height());
                    //Magick::Image mimage(mblob, mgeom);
                    //wxMessageBox(mimage.fileName());
                    //Magick::readImages(&imageList, mimage, options);
                    //Magick::readImages(&imageList, pfile, options);
                }
                //Magick::readImages(&imageList, m_param.pdffilename, options);
                //Magick::readImages(&imageList, "C:/1/Beginnin.pdf", options);
            /* }
            catch(std::exception &e)
            {
                wxMessageBox(e.what());
            }
            catch(Magick::WarningCoder& c)
            {
                wxMessageBox(c.what());
            }
            catch (Magick::Warning& w)
            {
                wxMessageBox(w.what());
            }
            catch (Magick::ErrorFileOpen& f)
            {
                wxMessageBox(f.what());
            }
            catch (Magick::Error& e)
            {
                wxMessageBox(e.what());
            }*/
            //wxMessageBox(wxString::Format("%d", static_cast<int>(imageList.size())));
            for (auto img = imageList.begin(); img != imageList.end(); img++)
            {
                if(m_param.convertToGray)
                    img->colorSpace(MagickCore::ColorspaceType::GRAYColorspace);
                //img->autoGamma();
                img->autoLevel();
                img->normalize();
                img->deskew(100.0);
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
