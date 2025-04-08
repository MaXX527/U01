// Start of wxWidgets "Hello World" Program

#include "Main.h"

wxIMPLEMENT_APP(MyApp);

/*class MyApp : public wxApp
{
public:
    bool OnInit() override;
};*/

bool MyApp::OnInit()
{
    const int len = 1000;
    char env_magick_home[len];
    char env_magick_coder[len];
    char env_magick_filter[len];
    ::_getcwd(env_magick_home, len);
    ::strcpy_s(env_magick_coder, env_magick_home);
    ::strcat_s(env_magick_coder, "\\modules\\coders");
    ::strcpy_s(env_magick_filter, env_magick_home);
    ::strcat_s(env_magick_filter, "\\modules\\filters");
    ::_putenv_s("MAGICK_HOME", env_magick_home);
    ::_putenv_s("MAGICK_CODER_MODULE_PATH", env_magick_coder);
    ::_putenv_s("MAGICK_FILTER_MODULE_PATH", env_magick_filter);
    //wxMessageBox(wxString(env_magick_coder) + "\r\n" + wxString(env_magick_filter));

    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Сжималка PDF и TIF")
{
    wxMenu* menuFile = new wxMenu;

    menuFile->Append(ID_MenuOpenDir, "Открыть каталог");
    Bind(wxEVT_MENU, &MyFrame::OpenDir, this, ID_MenuOpenDir);
    menuFile->Append(ID_MenuOpenFile, "Открыть файл");
    Bind(wxEVT_MENU, &MyFrame::OpenFile, this, ID_MenuOpenFile);
    menuFile->AppendSeparator();

    menuFile->Append(ID_Hello, "&Приветствие...\tCtrl-H", "Эта строка показывается в статусбаре при наведении на пункт меню");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Файл");
    menuBar->Append(menuHelp, "&Помощь");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Тест wxWidgets");

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

    buttonsSizer->Add(new wxButton(this, ID_OpenFile, "Добавить файл", wxDefaultPosition, wxDefaultSize), 1, 0, 10);
    Bind(wxEVT_BUTTON, &MyFrame::OpenFile, this, ID_OpenFile);

    buttonsSizer->Add(new wxButton(this, ID_OpenDir, "Добавить каталог", wxDefaultPosition, wxDefaultSize), 1, 0, 10);
    Bind(wxEVT_BUTTON, &MyFrame::OpenDir, this, ID_OpenDir);

    buttonsSizer->Add(new wxButton(this, ID_Clear, "Очистить", wxDefaultPosition, wxDefaultSize), 1, 0, 10);
    Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { m_list->DeleteAllItems(); }, ID_Clear);

    topSizer->Add(buttonsSizer);

    /*wxGridCellAttr* attr2 = new wxGridCellAttr();
    attr2->SetAlignment(wxALIGN_RIGHT, wxALIGN_TOP);
    wxGridCellAttr* attr3 = new wxGridCellAttr();
    attr3->SetAlignment(wxALIGN_RIGHT, wxALIGN_TOP);*/

    m_list = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(500, 150), wxDV_ROW_LINES | wxDV_MULTIPLE);
    m_list->AppendTextColumn("Тип");
    m_list->AppendTextColumn("Имя");
    m_list->AppendTextColumn("Размер");
    m_list->AppendTextColumn("Сжатый");
    m_list->AppendProgressColumn("Прогресс");

    //m_list->GetColumn(1)->SetWidth(300);
    m_list->GetColumn(2)->SetAlignment(wxALIGN_RIGHT);
    m_list->GetColumn(3)->SetAlignment(wxALIGN_RIGHT);

    m_list->Bind(wxEVT_KEY_DOWN, &MyFrame::OnDeleteItem, this);
    m_list->Bind(wxEVT_COMMAND_DATAVIEW_ITEM_CONTEXT_MENU, &MyFrame::OnContextMenu, this);

    topSizer->Add(m_list, 1, wxEXPAND | wxALL);

    wxBoxSizer* settingsSizer = new wxBoxSizer(wxHORIZONTAL);

    // Настройки TIF
    wxStaticBoxSizer* sizerTIF = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Настройки TIF"), wxDefaultPosition, wxDefaultSize, wxBORDER_DEFAULT), wxVERTICAL);

    m_fgTIFSizer = new wxFlexGridSizer(0, 2, 5, 5);

    m_fgTIFSizer->Add(new wxStaticText(this, wxID_ANY, "Размер"), 1, wxALL, 2);
    wxString rescaleChoices[] = { wxT("100%"), wxT("75%"), wxT("50%"), wxT("25%") };
    int rescaleNChoices = sizeof(rescaleChoices) / sizeof(wxString);
    m_choiceTIFrescale = new wxChoice(this, ID_choiceTIFrescale, wxDefaultPosition, wxDefaultSize, rescaleNChoices, rescaleChoices);
    m_choiceTIFrescale->SetSelection(0);
    m_fgTIFSizer->Add(m_choiceTIFrescale, 1, wxEXPAND | wxALL, 2);

    m_fgTIFSizer->Add(new wxStaticText(this, wxID_ANY, "Глубина цвета"), 1, wxALL, 2);
    wxString depthChoices[] = { wxT("1-bit"), wxT("4-bit"), wxT("8-bit") };
    int depthNChoices = sizeof(depthChoices) / sizeof(wxString);
    m_choiceTIFdepth = new wxChoice(this, ID_choiceTIFdepth, wxDefaultPosition, wxDefaultSize, depthNChoices, depthChoices);
    m_choiceTIFdepth->SetSelection(0);
    m_fgTIFSizer->Add(m_choiceTIFdepth, 1, wxEXPAND | wxALL, 2);
    
    sizerTIF->Add(m_fgTIFSizer);

    settingsSizer->Add(sizerTIF, 1, wxEXPAND | wxALL, 5);

    //Настройки PDF
    wxStaticBoxSizer* sizerPDF = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Настройки PDF"), wxDefaultPosition, wxDefaultSize, wxBORDER_DEFAULT), wxVERTICAL);

    m_fgPDFSizer = new wxFlexGridSizer(0, 3, 5, 5);

    m_checkPDFgray = new wxCheckBox(this, wxID_ANY, "Серый");
    m_fgPDFSizer->Add(m_checkPDFgray, 1, wxEXPAND | wxALL, 2);
    m_fgPDFSizer->Add(0, 0, 1, wxEXPAND, 5);
    m_fgPDFSizer->Add(0, 0, 1, wxEXPAND, 5);

    m_fgPDFSizer->Add(new wxStaticText(this, wxID_ANY, "Разрешение"), 1, wxALL, 2);
    m_spinPDFdpi = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 75, 600, 75);
    m_fgPDFSizer->Add(m_spinPDFdpi);
    m_fgPDFSizer->Add(0, 0, 1, wxEXPAND, 5);

    m_fgPDFSizer->Add(new wxStaticText(this, wxID_ANY, "Качество"), 1, wxALL, 2);
    m_sliderPDFquality = new wxSlider(this, wxID_ANY, 0, 0, 10, wxDefaultPosition, wxDefaultSize, wxSL_TICKS/* | wxSL_LABELS*/);
    m_sliderPDFquality->Bind(wxEVT_SCROLL_THUMBTRACK /*wxEVT_SCROLL_THUMBRELEASE*/, &MyFrame::OnSliderChange, this);
    //m_fgPDFSizer->Add(m_sliderPDFquality);
    //m_spinPDFquality = new wxSpinCtrl(this, ID_SpinQuality, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 10);
    m_fgPDFSizer->Add(m_sliderPDFquality);
    m_labelPDFquality = new wxStaticText(this, wxID_ANY, "%");
    m_fgPDFSizer->Add(m_labelPDFquality);

    sizerPDF->Add(m_fgPDFSizer);

    settingsSizer->Add(sizerPDF, 1, wxEXPAND | wxALL, 5);

    settingsSizer->Add(new wxButton(this, ID_PDF, "Сжать файлы", wxDefaultPosition, wxSize(20, 40)), 1, wxALIGN_CENTER_VERTICAL/*, wxEXPAND | wxALL */ );

    topSizer->Add(settingsSizer, 0, wxEXPAND | wxALL);
    SetSizer(topSizer);

    DragAcceptFiles(true);

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

    Bind(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrame::OnClose), this, wxID_ANY);

    Bind(wxEVT_DROP_FILES, wxDropFilesEventHandler(MyFrame::OnDropFiles), this, wxID_ANY);
    Bind(wxEVT_BUTTON, &MyFrame::OnClickPDF, this, ID_PDF);

    Bind(wxEVT_THREAD, wxThreadEventHandler(MyFrame::OnTIFCompressed), this, wxID_ANY);
    //Connect(ID_COMPRESSED, wxEVT_BUTTON, wxThreadEventHandler(MyFrame::OnTIFCompressed));
    Bind(wxEVT_SIZE, &MyFrame::OnSize, this, wxID_ANY);

    //m_spinPDFquality->Bind(wxEVT_SPIN_UP, wxSpinEventHandler(MyFrame::OnSpinQualityUp), this); // , ID_SpinQuality);
    //m_spinPDFquality->Bind(wxEVT_SPINCTRL, wxSpinEventHandler(MyFrame::OnSpinQualityUp), this);
    //m_spinPDFquality->Bind(wxEVT_SPIN_DOWN, wxSpinEventHandler(MyFrame::OnSpinQualityDown), this); //, ID_SpinQuality);
    
    SetMinSize(wxSize(700, 400));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
    SetIcon(wxIcon("WXICON_AAA"));

    LoadConfig();

    char** argv = wxGetApp().argv;
    Magick::InitializeMagick(*argv);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    delete this;
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    //wxMessageBox("Тест wxWidgets", "О программе", wxOK | wxICON_INFORMATION);
    DialogAbout dlg = new DialogAbout(this);
    dlg.ShowModal();
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Горячий привет от ОГАУ МФЦ Челябинской области");
}

wxString MyFrame::GetFileSize(const boost::filesystem::path& filename)
{
    if (!boost::filesystem::exists(filename))
        return "0 Б";
    const UINT chBuf = 10;
    char pszBuf[chBuf];
    uintmax_t filesize = boost::filesystem::file_size(filename);
    ::StrFormatByteSizeA(filesize, pszBuf, chBuf);
    return wxString(pszBuf);
    //char Buf[20];
    //_itoa(filesize, Buf, 10);
    //return wxString(Buf);
}

wxString MyFrame::PrintSize(DWORD bytes)
{
    UINT chBuf = 0;
    ::StrFormatByteSizeA(bytes, nullptr, chBuf);
    char *pszBuf = new char[chBuf];
    ::StrFormatByteSizeA(bytes, pszBuf, chBuf);
    return wxString(pszBuf);
}

void MyFrame::Check_File(const boost::filesystem::path& p) {
    wxVector<wxVariant> item;
    item.clear();
    std::string extension = p.extension().generic_string();
    if (".pdf" == extension)
    {
        item.push_back("pdf");
        item.push_back(p.string());
        item.push_back(GetFileSize(p));
        item.push_back("");
        item.push_back(0);
        m_list->AppendItem(item);
    }
    if (".tif" == extension || ".tiff" == extension)
    {
        item.push_back("tif");
        item.push_back(p.string());
        item.push_back(GetFileSize(p));
        item.push_back("");
        item.push_back(0);
        m_list->AppendItem(item);
    }
}

void MyFrame::OnDropFiles(wxDropFilesEvent& event)
{
    if (event.GetNumberOfFiles() > 0) {
        wxString* files = event.GetFiles();
        for (int i = 0; i < event.GetNumberOfFiles(); i++)
        {
            //wxMessageBox(files[i].ToStdString());
            if (boost::filesystem::is_regular_file(files[i].ToStdString())) {
                Check_File(boost::filesystem::path(files[i].ToStdString()));
            }
            if (boost::filesystem::is_directory(files[i].ToStdString())) {
                boost::filesystem::recursive_directory_iterator dir(boost::filesystem::path(files[i].ToStdString())), end;
                while (dir != end)
                {
                    Check_File(dir->path());
                    dir++;
                }
            }
        }
    }
}

void MyFrame::OnClickPDF(wxCommandEvent& event)
{
    if (0 == m_list->GetItemCount())
    {
        wxMessageBox("Нет файлов для сжатия");
        return;
    }
    //std::list<Magick::Image> imageList;
    //Magick::Image image;
    //char percent[10] = "";
    for (int i = 0; i < m_list->GetItemCount(); i++)
    {
        TIFparam param;
        //ZeroMemory(&param, sizeof(param));
        //wxVariant varfilename;
        //m_list->GetValue(varfilename, i, 1);
        //std::string filename = varfilename.GetString().ToStdString();
        std::string filename = GetDataViewListString(i, 1).ToStdString();
        boost::filesystem::path path = boost::filesystem::path(filename);
        //std::string newfilename = (path.parent_path().append(path.stem()).concat("_compress").concat(path.extension())).string();
        std::string newfilename = (path.parent_path().append(path.stem()).concat("_compress").concat(path.extension())).string();

        m_timers.Put(i, new wxTimer());
        //MyUserData* userData = new MyUserData();
        //userData->data = &i;
        ((wxTimer*)(m_timers.Get(i)))->Bind(wxEVT_TIMER, [=](wxTimerEvent& event)
            {
                wxString data = GetDataViewListString(i, 3) + ".";
                if (data.length() > 5)
                    data = ".";
                m_list->SetValue(data, i, 3);
            } , wxID_ANY);
        ((wxTimer*)(m_timers.Get(i)))->Start(500);
        m_list->SetValue(".", i, 3);

        if("tif" == GetDataViewListString(i, 0))
        {
            param.rowNumber = i;
            param.type = "tif";
            //param.compressType = Magick::CompressionType::LZWCompression;
            param.filename = filename;
            switch (m_choiceTIFdepth->GetSelection())
            {
            case 0:
                param.depth = 1;
                break;
            case 1:
                param.depth = 4;
                break;
            case 2:
                param.depth = 8;
                break;
            }
            //param.autoGamma = false;
            //param.autoLevel = true;
            //param.normalize = true;
            //param.strip = true;
            switch (m_choiceTIFrescale->GetSelection())
            {
                case 0:
                    param.rescale = 1.0f;
                    break;
                case 1:
                    param.rescale = 0.75f;
                    break;
                case 2:
                    param.rescale = 0.5f;
                    break;
                case 3:
                    param.rescale = 0.25f;
                    break;
            }
            param.newfilename = newfilename;

            MyThread* thread = new MyThread(param, 0);
            thread->Create();
            thread->Run();

            /*try
            {
                int n = 0;
                Magick::readImages(&imageList, grid->GetCellValue(i, 1).ToStdString());
                for (auto img = imageList.begin(); img != imageList.end(); img++)
                {
                    //img->compressType(Magick::CompressionType::Group4Compression - BW JPEG Magick::CompressionType::LZWCompression);
                    img->depth(4);
                    //img->autoGamma();
                    img->autoLevel();
                    img->normalize();
                    img->strip();
                    //img->randomThreshold(30.0, 70.0);
                    //img->threshold(100.0);
                    wxSprintf(percent, "%0.2f %%", 100.0 * (++n) / imageList.size());
                    grid->SetCellValue(i, 3, percent);
                    this->Update();
                }
                boost::filesystem::path path = boost::filesystem::path(grid->GetCellValue(i, 1));
                std::string newfilename = path.parent_path().append(path.stem()).concat("_compress").concat(path.extension()).string();
                Magick::writeImages(imageList.begin(), imageList.end(), newfilename);
                grid->SetCellValue(i, 3, GetFileSize(boost::filesystem::path(newfilename)));
            }
            catch (Magick::Error err)
            {
                wxMessageBox(err.what());
            }
            imageList.clear();*/
        }

        //C:\Program Files\ImageMagick-7.1.1-Q16>magick convert -colorspace gray -density 120 -compress jpeg -quality 70 "C:\tmp\1 2 3.pdf" "C:\tmp\123.pdf"
        if("pdf" == GetDataViewListString(i, 0))
        {
            /*std::list<Magick::Image> imageList;
            //Magick::Image image;
            char percent[10] = "";
            try
            {
                int n = 0;
                Magick::ReadOptions options;
                options.density(Magick::Geometry(300, 300));
                options.depth(4);
                Magick::readImages(&imageList, GetDataViewListString(i, 1).ToStdString(), options);
                for (auto img = imageList.begin(); img != imageList.end(); img++)
                {
                    img->colorSpace(MagickCore::ColorspaceType::GRAYColorspace);
                    //img->resize(Magick::Geometry("2548x3508"));
                    //img->interlaceType(MagickCore::InterlaceType::NoInterlace);
                    //img->density(100);
                    //img->scale("@1500000");
                    img->compressType(Magick::CompressionType::JPEGCompression);
                    img->quality(20);
                    img->resample(96);
                    wxSprintf(percent, "%0.2f %%", 100.0 * (++n) / imageList.size());
                    m_list->SetValue(percent, i, 3);
                    this->Update();
                }
                boost::filesystem::path path = boost::filesystem::path(GetDataViewListString(i, 1).ToStdString());
                std::string newfilename = path.parent_path().append(path.stem()).concat("_compress").concat(path.extension()).string();
                Magick::writeImages(imageList.begin(), imageList.end(), newfilename);
                m_list->SetValue(GetFileSize(boost::filesystem::path(newfilename)), i, 3);
            }
            catch (Magick::Error err)
            {
                wxMessageBox(err.what());
            }
            imageList.clear();*/

            param.rowNumber = i;
            param.type = "pdf";
            //param.compressType = Magick::CompressionType::LZWCompression;
            param.pdffilename = wxString::Format("%s\\temppdf-%d.pdf", boost::filesystem::path(filename).parent_path().string(), i);
            param.filename = filename; // grid->GetCellValue(i, 1).ToStdString();
            wxCopyFile(param.filename, param.pdffilename);
            //param.depth = 4;
            //param.autoGamma = false;
            //param.autoLevel = true;
            //param.normalize = true;
            //param.strip = true;
            //param.rescale = false;
            param.pdfnewfilename = wxString::Format("%s\\temppdf-%d_compress.pdf", boost::filesystem::path(filename).parent_path().string(), i);
            param.newfilename = newfilename;
            param.resolution = m_spinPDFdpi->GetValue();
            param.convertToGray = m_checkPDFgray->GetValue();
            param.quality = m_sliderPDFquality->GetValue();

            //CoInitialize(NULL);

            //CoUninitialize();

            MyThread* thread = new MyThread(param, 1);
            thread->Create();
            thread->Run();
            
            //int exit_code = param.exit_code;

            //char msg[20];
            //wxSprintf(msg, "Error code %d", exit_code);

        }
    }
}

void MyFrame::OnTIFCompressed(wxThreadEvent& event)
{
    ((wxTimer*)(m_timers.Get(event.GetPayload<TIFparam>().rowNumber)))->Stop();
    //wxMessageBox(wxString::Format("%d", event.GetPayload<TIFparam>().percent));
    if (-1 == event.GetPayload<TIFparam>().percent)
    {
        if ("pdf" == event.GetPayload<TIFparam>().type)
        {
            wxRenameFile(event.GetPayload<TIFparam>().pdfnewfilename, event.GetPayload<TIFparam>().newfilename);
            wxRemoveFile(event.GetPayload<TIFparam>().pdffilename);
        }
        m_list->SetValue(GetFileSize(boost::filesystem::path(event.GetPayload<TIFparam>().newfilename)), event.GetPayload<TIFparam>().rowNumber, 3);
        m_list->SetValue(0, event.GetPayload<TIFparam>().rowNumber, 4);
    }
    else
    {
        m_list->SetValue(event.GetPayload<TIFparam>().percent, event.GetPayload<TIFparam>().rowNumber, 4);
        this->Update();
    }
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    wxStatusBar *sb = this->GetStatusBar();
    sb->SetStatusText(wxString::Format("%d x %d", event.GetSize().GetWidth(), event.GetSize().GetHeight()));
    m_list->GetColumn(0)->SetWidth(50);
    m_list->GetColumn(2)->SetWidth(75);
    m_list->GetColumn(3)->SetWidth(75);
    m_list->GetColumn(4)->SetWidth(75);
    m_list->GetColumn(1)->SetWidth(this->GetClientSize().GetWidth() - 50 - 75 - 75 - 75 - 10);
    event.Skip();
}

wxString MyFrame::GetDataViewListString(int row, int col)
{
    wxVariant val;
    m_list->GetValue(val, row, col);
    return val.GetString();
}

void MyFrame::OnClose(wxCloseEvent& event)
{
    //wxMessageBox("OnClose");
    SaveConfig();
    //delete this;
    event.Skip();
}

void MyFrame::LoadConfig()
{
    m_config = new wxConfig("U01", "OGAU MFC");
    m_config->SetPath("/tiff");
    int depth = 0;
    m_config->Read("depth", &depth);
    m_choiceTIFdepth->SetSelection(depth);
    int rescale = 0;
    m_config->Read("rescale", &rescale);
    m_choiceTIFrescale->SetSelection(rescale);
    m_config->SetPath("/pdf");
    int dpi = 0;
    m_config->Read("dpi", &dpi);
    m_spinPDFdpi->SetValue(dpi);
    bool gray = false;
    m_config->Read("gray", &gray);
    m_checkPDFgray->SetValue(gray);
    int quality = 0;
    m_config->Read("quality", &quality);
    m_sliderPDFquality->SetValue(quality);
    m_labelPDFquality->SetLabelText(wxString::Format("%d %%", quality * 10));
}

void MyFrame::SaveConfig()
{
    m_config->SetPath("/tiff");
    m_config->Write("depth", m_choiceTIFdepth->GetSelection());
    m_config->Write("rescale", m_choiceTIFrescale->GetSelection());
    m_config->SetPath("/pdf");
    m_config->Write("dpi", m_spinPDFdpi->GetValue());
    m_config->Write("gray", m_checkPDFgray->GetValue());
    m_config->Write("quality", m_sliderPDFquality->GetValue());
}

void MyFrame::OpenFile(wxCommandEvent& event)
{
    wxFileDialog* dlg = new wxFileDialog(this, "Выберите файлы", wxEmptyString, wxEmptyString, "Файлы для сжатия (*.tif,*.tiff,*.pdf)|*.tif;*.tiff;*.pdf", wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST | wxFD_OPEN);
    if (wxID_CANCEL == dlg->ShowModal())
        return;
    wxArrayString files;
    dlg->GetFilenames(files);
    for (auto f = files.begin(); f < files.end(); f++)
    {
        Check_File(boost::filesystem::path(f->ToStdString()));
    }
}

void MyFrame::OpenDir(wxCommandEvent& event)
{
    wxDirDialog* dlg = new wxDirDialog(this, "Выберите каталог", wxEmptyString, wxDD_DIR_MUST_EXIST);
    if (wxID_CANCEL == dlg->ShowModal())
        return;
    try
    {
        boost::filesystem::recursive_directory_iterator dir(boost::filesystem::path(dlg->GetPath().ToStdString())), end;
        while (dir != end)
        {
            Check_File(dir->path());
            dir++;
        }
    }
    catch (boost::filesystem::filesystem_error e)
    {
        wxMessageBox(e.what());
    }
}

void MyFrame::OnDeleteItem(wxKeyEvent& event)
{
    if (wxKeyCode::WXK_DELETE == event.GetKeyCode() || wxKeyCode::WXK_NUMPAD_DELETE == event.GetKeyCode())
    {
        wxDataViewItemArray sel;
        m_list->GetSelections(sel);
        for (auto item = sel.begin(); item != sel.end(); item++)
        {
            m_list->DeleteItem(m_list->ItemToRow(*item));
        }
    }
    //wxMessageBox(wxString::Format("%d", event.GetKeyCode()));
    event.Skip();
}

/*void MyFrame::OnSpinQualityUp(wxSpinEvent& event)
{
    static int prevValue = 10;
    m_spinPDFquality->SetValue(m_spinPDFquality->GetValue() + 10 * (event.GetValue() - prevValue));
    prevValue = m_spinPDFquality->GetValue();
}/*

/*void MyFrame::OnSpinQualityDown(wxSpinEvent& event)
{
    wxMessageBox("down");
    m_spinPDFquality->SetValue(event.GetValue() - 10);
}*/

void MyFrame::OnSliderChange(wxScrollEvent& event)
{
    //wxMessageBox(wxString::Format("Scroll %d", event.GetInt()));
    m_labelPDFquality->SetLabelText(wxString::Format("%d %%", event.GetInt() * 10));
}

/*void MyFrame::OpenOldFile(wxEvent& event)
{
    boost::filesystem::path path = boost::filesystem::path(GetDataViewListString(m_list->GetSelectedRow(), 1));
    if (boost::filesystem::exists(path))
        wxLaunchDefaultApplication(path.string(), wxEXEC_SYNC);
}

void MyFrame::OpenNewFile(wxEvent& event)
{
    boost::filesystem::path path = boost::filesystem::path(GetDataViewListString(m_list->GetSelectedRow(), 1));
    path = (path.parent_path().append(path.stem()).concat("_compress").concat(path.extension()));
    if(boost::filesystem::exists(path))
        wxLaunchDefaultApplication(path.string(), wxEXEC_SYNC);
}*/

void MyFrame::OnContextMenu(wxDataViewEvent& event)
{
    if (0 == m_list->GetItemCount())
        return;
    wxMenu* popupMenu = new wxMenu;
    if (event.GetItem())
    {
        popupMenu->Append(ID_PopupOpenOld, "Открыть исходный файл");
        //Bind(wxEVT_MENU, &MyFrame::OpenOldFile, this, ID_PopupOpenOld);
        Bind(wxEVT_MENU, [=](wxEvent& event)
            {
                boost::filesystem::path path = boost::filesystem::path(GetDataViewListString(m_list->GetSelectedRow(), 1));
                if (boost::filesystem::exists(path))
                    wxLaunchDefaultApplication(path.string(), wxEXEC_SYNC); 
            }, ID_PopupOpenOld);
        popupMenu->Append(ID_PopupOpenNew, "Открыть сжатый файл");
        //Bind(wxEVT_MENU, &MyFrame::OpenNewFile, this, ID_PopupOpenNew);
        Bind(wxEVT_MENU, [=](wxEvent& event)
            {
                boost::filesystem::path path = boost::filesystem::path(GetDataViewListString(m_list->GetSelectedRow(), 1));
                path = (path.parent_path().append(path.stem()).concat("_compress").concat(path.extension()));
                if (boost::filesystem::exists(path))
                    wxLaunchDefaultApplication(path.string(), wxEXEC_SYNC);
            }, ID_PopupOpenNew);
        popupMenu->AppendSeparator();
        if(1 == m_list->GetSelectedItemsCount())
            popupMenu->Append(ID_PopupDeleteRow, "Удалить строку");
        else
            popupMenu->Append(ID_PopupDeleteRow, "Удалить выбранные строки");
        // Bind(wxEVT_MENU, [=](wxEvent&) { m_list->DeleteItem(m_list->GetSelectedRow()); }, ID_PopupDeleteRow);

        Bind(wxEVT_MENU, [=](wxEvent&) { wxKeyEvent evt; evt.m_keyCode = wxKeyCode::WXK_DELETE; OnDeleteItem(evt); }, ID_PopupDeleteRow);
    }
    popupMenu->Append(ID_PopupClear, "Удалить всё");
    Bind(wxEVT_MENU, [=](wxEvent&) { m_list->DeleteAllItems(); }, ID_PopupClear);
    m_list->PopupMenu(popupMenu);
}
