
// HandGestureRecognitionSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HandGestureRecognitionSystem.h"
#include "HandGestureRecognitionSystemDlg.h"
#include "afxdialogex.h"

// Numeric constants
const short ID_TIMER = 1;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHandGestureRecognitionSystemDlg dialog




CHandGestureRecognitionSystemDlg::CHandGestureRecognitionSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHandGestureRecognitionSystemDlg::IDD, pParent)
    , image_height(240)
    , image_width(320)
    , image_interval(20)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    videocapture = VideoCapture(0);
    image_src = Mat(image_height, image_width, CV_8UC3);
    image_dst = Mat(image_height, image_width, CV_8UC3);
}

void CHandGestureRecognitionSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHandGestureRecognitionSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONREADIMAGE, &CHandGestureRecognitionSystemDlg::OnClickedButtonPlayCamera)
    ON_BN_CLICKED(IDC_BUTTONPROCESSIMAGE, &CHandGestureRecognitionSystemDlg::OnClickedButtonStopCamera)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CHandGestureRecognitionSystemDlg message handlers

BOOL CHandGestureRecognitionSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHandGestureRecognitionSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHandGestureRecognitionSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
    // Set area
    MoveWindow(image_interval,
        image_interval,
        image_width * 2 + 4 * image_interval,
        image_height + 8 * image_interval);
    GetDlgItem(IDC_IMAGESRC)->MoveWindow(image_interval * 1,
        image_interval * 1,
        image_width,
        image_height);
    GetDlgItem(IDC_IMAGEDST)->MoveWindow(image_interval * 2 + image_width,
        image_interval * 1,
        image_width,
        image_height);
    GetDlgItem(IDC_BUTTONREADIMAGE)->MoveWindow(image_interval * 2,
        image_interval * 2 + image_height,
        image_interval * 4,
        image_interval);
    GetDlgItem(IDC_BUTTONPROCESSIMAGE)->MoveWindow(image_interval * 6,
        image_interval * 2 + image_height,
        image_interval * 4,
        image_interval);
    GetDlgItem(IDC_STATIC)->MoveWindow(image_interval * 12,
        image_interval * 2 + image_height,
        image_interval * 8,
        image_interval);
    CFont testfont;
    testfont.CreatePointFont(125, "微软雅黑");
    GetDlgItem(IDC_STATIC)->SetFont(&testfont);
    GetDlgItem(IDOK)->MoveWindow(image_width * 2 - image_interval * 3,
        image_interval * 2 + image_height,
        image_interval * 2,
        image_interval);
    GetDlgItem(IDCANCEL)->MoveWindow(image_width * 2,
        image_interval * 2 + image_height,
        image_interval * 2,
        image_interval);
    // set rect
    GetDlgItem(IDC_IMAGESRC)->GetClientRect(&rect_image_src);
    GetDlgItem(IDC_IMAGEDST)->GetClientRect(&rect_image_dst);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHandGestureRecognitionSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHandGestureRecognitionSystemDlg::OnClickedButtonPlayCamera()
{
    // TODO: Add your control notification handler code here
    if(!GetImageSrc()) {
        AfxMessageBox("CAN NOT FIND CAMERA DEVICE!");
        return;
    }
    ProcessImage();
    PlayImageSrc();
    PlayImageDst();
    StartTimer();
}


void CHandGestureRecognitionSystemDlg::OnClickedButtonStopCamera()
{
    // TODO: Add your control notification handler code here
    StopTimer();
}


void CHandGestureRecognitionSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if(!GetImageSrc()) {
        AfxMessageBox("CAN NOT FIND CAMERA DEVICE!");
        return;
    }
    ProcessImage();
    PlayImageSrc();
    PlayImageDst();
    CDialogEx::OnTimer(nIDEvent);
}

// CHandGestureRecognitionSystemDlg::StartTimer - Start the timer.

void CHandGestureRecognitionSystemDlg::StartTimer(void)
{
    // timer
    SetTimer(ID_TIMER, 30, NULL);
}

// CHandGestureRecognitionSystemDlg::StopTimer - Stop the timer.

void CHandGestureRecognitionSystemDlg::StopTimer(void)
{
    // kill timer
    KillTimer(ID_TIMER);
}


void CHandGestureRecognitionSystemDlg::PlayImageSrc(void)
{
    // get HDC
    CDC* pDC = GetDlgItem(IDC_IMAGESRC)->GetDC(); // 获得显示控件的 DC
    HDC hDC = pDC ->GetSafeHdc();                 // 获取HDC(设备句柄)进行绘图操作
    // MAT TO IplImage
    IplImage img = IplImage(image_src);
    // IplImage TO CvvImage
    CvvImage cimg;
    cimg.CopyOf(&img);                          // 复制图片
    cimg.DrawToHDC(hDC, &rect_image_src);       // 将图片绘制到显示控件的指定区域
    // release CDC
    ReleaseDC(pDC);
}


void CHandGestureRecognitionSystemDlg::PlayImageDst(void)
{
    // get HDC
    CDC* pDC = GetDlgItem(IDC_IMAGEDST)->GetDC(); // 获得显示控件的 DC
    HDC hDC = pDC ->GetSafeHdc();                 // 获取HDC(设备句柄)进行绘图操作
    // MAT TO IplImage
    IplImage img = IplImage(image_dst);
    // IplImage TO CvvImage
    CvvImage cimg;
    cimg.CopyOf(&img);                          // 复制图片
    cimg.DrawToHDC(hDC, &rect_image_dst);       // 将图片绘制到显示控件的指定区域
    // release CDC
    ReleaseDC(pDC);
}


void CHandGestureRecognitionSystemDlg::ProcessImage(void)
{
    resize(image_src, image_src, Size(image_width, image_height));
    Canny(image_src, image_dst, 5,5);
}


bool CHandGestureRecognitionSystemDlg::GetImageSrc(void)
{
    if(!videocapture.isOpened())   // check if succeeded
        return false;
    videocapture >> image_src;
    if (image_src.data == NULL)
        return false;
    else
        return true;
}
