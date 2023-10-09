// Start of wxWidgets "Hello World" Program

#include "Main.h"

wxIMPLEMENT_APP(MyApp);


/*class MyApp : public wxApp
{
public:
    bool OnInit() override;
};*/

enum
{
    ID_Hello = 1,
    ID_PDF,
    ID_choiceTIFrescale,
    ID_choiceTIFdepth,
    ID_Clear,
    ID_OpenFile,
    ID_OpenDir
};

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Сжималка PDF и TIF")
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

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

    //m_list->GetColumn(1)->SetWidth(300);
    m_list->GetColumn(2)->SetAlignment(wxALIGN_RIGHT);
    m_list->GetColumn(3)->SetAlignment(wxALIGN_RIGHT);

    m_list->Bind(wxEVT_KEY_DOWN, &MyFrame::OnDeleteItem, this);

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

    m_fgPDFSizer = new wxFlexGridSizer(0, 2, 5, 5);

    m_checkPDFgray = new wxCheckBox(this, wxID_ANY, "Серый");
    m_fgPDFSizer->Add(m_checkPDFgray, 1, wxEXPAND | wxALL, 2);
    m_fgPDFSizer->Add(0, 0, 1, wxEXPAND, 5);

    m_fgPDFSizer->Add(new wxStaticText(this, wxID_ANY, "Разрешение"), 1, wxALL, 2);
    m_spinPDFdpi = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 75, 600, 75);
    m_fgPDFSizer->Add(m_spinPDFdpi);

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

    SetMinSize(wxSize(700, 400));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));

    LoadConfig();

    //char** argv = wxGetApp().argv;
    //Magick::InitializeMagick(*argv);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    delete this;
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
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
        m_list->AppendItem(item);
    }
    if (".tif" == extension || ".tiff" == extension)
    {
        item.push_back("tif");
        item.push_back(p.string());
        item.push_back(GetFileSize(p));
        item.push_back("");
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

        if("tif" == GetDataViewListString(i, 0))
        {
            param.rowNumber = i;
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
        //if ("pdf" == grid->GetCellValue(i, 0))
        if("pdf" == GetDataViewListString(i, 0))
        {
            /*try
            {
                int n = 0;
                Magick::readImages(&imageList, grid->GetCellValue(i, 1).ToStdString());
                for (auto img = imageList.begin(); img != imageList.end(); img++)
                {
                    img->colorSpace(MagickCore::ColorspaceType::RGBColorspace);
                    img->resize(Magick::Geometry("2548x3508"));
                    img->interlaceType(MagickCore::InterlaceType::NoInterlace);
                    img->density(300);
                    //img->scale("@1500000");
                    //img->compressType(Magick::CompressionType::JPEGCompression);
                    img->quality(80);
                    //img->resample(360);
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

            param.rowNumber = i;
            //param.compressType = Magick::CompressionType::LZWCompression;
            param.filename = filename; // grid->GetCellValue(i, 1).ToStdString();
            //param.depth = 4;
            //param.autoGamma = false;
            //param.autoLevel = true;
            //param.normalize = true;
            //param.strip = true;
            //param.rescale = false;
            param.newfilename = newfilename;
            param.resolution = m_spinPDFdpi->GetValue();
            param.convertToGray = m_checkPDFgray->GetValue();
            MyThread* thread = new MyThread(param, 1);
            thread->Create();
            thread->Run();

            int exit_code = param.exit_code;

            /*char msg[20];
            wxSprintf(msg, "Error code %d", exit_code);
            if(boost::filesystem::exists(newfilename))
                grid->SetCellValue(i, 3, MyFrame::GetFileSize(boost::filesystem::path(newfilename)));
            else
                grid->SetCellValue(i, 3, msg);*/
            
        }
    }
}

void MyFrame::OnTIFCompressed(wxThreadEvent& event)
{
    //wxMessageBox(event.GetPayload<TIFparam>().newfilename);
    m_list->SetValue(GetFileSize(boost::filesystem::path(event.GetPayload<TIFparam>().newfilename)), event.GetPayload<TIFparam>().rowNumber, 3);
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    wxStatusBar *sb = this->GetStatusBar();
    sb->SetStatusText(wxString::Format("%d x %d", event.GetSize().GetWidth(), event.GetSize().GetHeight()));
    m_list->GetColumn(0)->SetWidth(50);
    m_list->GetColumn(2)->SetWidth(75);
    m_list->GetColumn(3)->SetWidth(75);
    m_list->GetColumn(1)->SetWidth(this->GetClientSize().GetWidth() - 50 - 75 - 75 - 10);
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
}

void MyFrame::SaveConfig()
{
    m_config->SetPath("/tiff");
    m_config->Write("depth", m_choiceTIFdepth->GetSelection());
    m_config->Write("rescale", m_choiceTIFrescale->GetSelection());
    m_config->SetPath("/pdf");
    m_config->Write("dpi", m_spinPDFdpi->GetValue());
    m_config->Write("gray", m_checkPDFgray->GetValue());
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