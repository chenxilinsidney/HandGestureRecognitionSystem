
// HandGestureRecognitionSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HandGestureRecognitionSystem.h"
#include "HandGestureRecognitionSystemDlg.h"
#include "afxdialogex.h"

// Numeric constants
const short ID_TIMER = 1;

// CHandGestureRecognitionSystemDlg dialog


CHandGestureRecognitionSystemDlg::CHandGestureRecognitionSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHandGestureRecognitionSystemDlg::IDD, pParent)
    , image_height(240)
    , image_width(320)
    , image_interval(20)
    , point_begin(image_width / 2, image_height / 2)
    , point_end(image_width / 2, image_height / 2)
    , vector_angle(0)
    , vector_length(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    videocapture = VideoCapture(0);
    image_camera = Mat(image_height, image_width, CV_8UC3);
    image_segment = Mat(image_height, image_width, CV_8UC3);
    image_feature = Mat(image_height, image_width, CV_8UC3);
    image_recognition = Mat(image_height, image_width, CV_8UC3);
    image_background = Mat(image_height, image_width, CV_8UC3);
}

void CHandGestureRecognitionSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHandGestureRecognitionSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_PLAY_CAMERA, &CHandGestureRecognitionSystemDlg::OnClickedButtonPlayCamera)
    ON_BN_CLICKED(IDC_BUTTON_STOP_CAMERA, &CHandGestureRecognitionSystemDlg::OnClickedButtonStopCamera)
    ON_WM_TIMER()
    ON_BN_CLICKED(ID_CLOSE, &CHandGestureRecognitionSystemDlg::OnClickedClose)
END_MESSAGE_MAP()


// CHandGestureRecognitionSystemDlg message handlers

BOOL CHandGestureRecognitionSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

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
			// pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
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
    CDialogEx::OnSysCommand(nID, lParam);
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
    // set area
    int static_height = image_interval * 1;
    int static_width = image_interval * 20;
    MoveWindow(image_interval,
        image_interval,
        image_width * 2 + 4 * image_interval,
        image_height * 2 + 20 * image_interval);
    GetDlgItem(IDC_STATIC_CAMERA)->MoveWindow(
        image_interval * 1 + (image_width - static_width) / 2,
        image_interval * 1,
        static_width,
        static_height);
    GetDlgItem(IDC_STATIC_CAMERA)->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
    GetDlgItem(IDC_IMAGE_CAMERA)->MoveWindow(
        image_interval * 1,
        image_interval * 3,
        image_width,
        image_height);
    GetDlgItem(IDC_STATIC_SEGMENT)->MoveWindow(
        image_interval * 1 + image_width + (image_width - static_width) / 2,
        image_interval * 1,
        static_width,
        static_height);
    GetDlgItem(IDC_STATIC_SEGMENT)->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
    GetDlgItem(IDC_IMAGE_SEGMENT)->MoveWindow(
        image_interval * 2 + image_width,
        image_interval * 3,
        image_width,
        image_height);
    GetDlgItem(IDC_STATIC_FEATURE)->MoveWindow(
        image_interval * 1 + (image_width - static_width) / 2,
        image_interval * 4 + image_height,
        static_width,
        static_height);
    GetDlgItem(IDC_STATIC_FEATURE)->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
    GetDlgItem(IDC_IMAGE_FEATURE)->MoveWindow(
        image_interval * 1,
        image_interval * 6 + image_height,
        image_width,
        image_height);
    GetDlgItem(IDC_STATIC_RECOGNITION)->MoveWindow(
        image_interval * 1 + image_width + (image_width - static_width) / 2,
        image_interval * 4 + image_height,
        static_width,
        static_height);
    GetDlgItem(IDC_STATIC_RECOGNITION)->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
    GetDlgItem(IDC_IMAGE_RECOGNITION)->MoveWindow(
        image_interval * 2 + image_width,
        image_interval * 6 + image_height,
        image_width,
        image_height);
    GetDlgItem(IDC_BUTTON_PLAY_CAMERA)->MoveWindow(
        image_interval * 1,
        image_interval * 7 + image_height * 2,
        image_interval * 5,
        image_interval);
    GetDlgItem(IDC_BUTTON_STOP_CAMERA)->MoveWindow
        (image_interval * 7,
        image_interval * 7 + image_height * 2,
        image_interval * 5,
        image_interval);
    GetDlgItem(IDC_BUTTON_REFRESH_BACKGROUND)->MoveWindow(
        image_interval * 13,
        image_interval * 7 + image_height * 2,
        image_interval * 5,
        image_interval);
    GetDlgItem(ID_CLOSE)->MoveWindow(
        image_interval * 22,
        image_interval * 7 + image_height * 2,
        image_interval * 5,
        image_interval);
    // set font
    CFont testfont;
    testfont.CreatePointFont(110, "微软雅黑");
    GetDlgItem(IDC_STATIC_CAMERA)->SetFont(&testfont);
    GetDlgItem(IDC_STATIC_SEGMENT)->SetFont(&testfont);
    GetDlgItem(IDC_STATIC_FEATURE)->SetFont(&testfont);
    GetDlgItem(IDC_STATIC_RECOGNITION)->SetFont(&testfont);
    GetDlgItem(ID_CLOSE)->SetFont(&testfont);
    // set rect
    GetDlgItem(IDC_IMAGE_CAMERA)->GetClientRect(&rect_image_camera);
    GetDlgItem(IDC_IMAGE_SEGMENT)->GetClientRect(&rect_image_segment);
    GetDlgItem(IDC_IMAGE_FEATURE)->GetClientRect(&rect_image_feature);
    GetDlgItem(IDC_IMAGE_RECOGNITION)->GetClientRect(&rect_image_recognition);
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
    // initialize camera and image
    if(!InitCameraAndImage())
        return;
    // start timer;
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
    if(!InitCameraAndImage())
        return;
    // play image camera
    PlayImage(image_camera, IDC_IMAGE_CAMERA, rect_image_camera);
    // segment
    image_segment = HandSegment(image_camera, image_segment);
    PlayImage(image_segment, IDC_IMAGE_SEGMENT, rect_image_segment);
    // feature detect
    image_feature = FeatureDetect(image_segment, image_feature);
    PlayImage(image_feature, IDC_IMAGE_FEATURE, rect_image_feature);
    // recognition
    image_recognition = Recognition(image_feature, image_recognition);
    PlayImage(image_recognition, IDC_IMAGE_RECOGNITION, rect_image_recognition);

    CDialogEx::OnTimer(nIDEvent);
}

// CHandGestureRecognitionSystemDlg::StartTimer - Start the timer.

void CHandGestureRecognitionSystemDlg::StartTimer(void)
{
    // set timer
    SetTimer(ID_TIMER, 30, NULL);
}

// CHandGestureRecognitionSystemDlg::StopTimer - Stop the timer.

void CHandGestureRecognitionSystemDlg::StopTimer(void)
{
    // kill timer
    KillTimer(ID_TIMER);
}

// Initial camera and image

bool CHandGestureRecognitionSystemDlg::InitCameraAndImage(void)
{
    if(!videocapture.isOpened()) {
        AfxMessageBox("CAN NOT FIND CAMERA DEVICE!");
        return false;
    }
    videocapture >> image_camera;
    if (image_camera.data == NULL) {
        AfxMessageBox("CAN NOT TRANS CAMERA IMAGE!");
        return false;
    } else {
        // resize the image
        resize(image_camera, image_camera, Size(image_width, image_height));
        image_segment = image_camera;
        image_feature = image_camera;
        image_recognition = image_camera;
        image_background = image_camera;
        return true;
    }
}

// display the image

void CHandGestureRecognitionSystemDlg::PlayImage(Mat& image, int ID, CRect& rect)
{
    // get HDC
    CDC* pDC = GetDlgItem(ID)->GetDC(); // 获得显示控件的 DC
    HDC hDC = pDC ->GetSafeHdc();       // 获取HDC(设备句柄)进行绘图操作
    // MAT TO IplImage
    IplImage img = IplImage(image);
    // IplImage TO CvvImage
    CvvImage cimg;
    cimg.CopyOf(&img);                  // 复制图片
    cimg.DrawToHDC(hDC, &rect);         // 将图片绘制到显示控件的指定区域
    // release CDC
    ReleaseDC(pDC);
}

// hand gesture segment

Mat& CHandGestureRecognitionSystemDlg::HandSegment(const Mat& image_input, Mat& image_output)
{
    image_output = image_input;
    return image_output;
}

// hand gesture feature detect

Mat& CHandGestureRecognitionSystemDlg::FeatureDetect(const Mat& image_input, Mat& image_output)
{
    image_output = image_input;
    //point_begin = Point(0, 0);
    //point_end = Point(0, 0);
    //GetVector(point_begin, point_end);
    return image_output;
}

// hand gesture recognition

Mat& CHandGestureRecognitionSystemDlg::Recognition(const Mat& image_input, Mat& image_output)
{
    image_output = image_input;
    return image_output;
}


// get vector value
void CHandGestureRecognitionSystemDlg::GetVector(Point point_begin, Point point_end)
{
    if(point_begin.x == point_end.x && point_begin.y == point_end.y) {
        vector_angle = 0;
        vector_length = 0;
    } else if(point_begin.x == point_end.x) {
        if (point_begin.y > point_end.y) {
            vector_angle = 90;
        } else {
            vector_angle = 270;
        }
        vector_length = std::fabs((double)point_begin.y - point_end.y);
    } else if(point_begin.y == point_end.y) {
        if (point_begin.x > point_end.x) {
            vector_angle = 180;
        } else {
            vector_angle = 0;
        }
        vector_length = std::fabs((double)point_begin.x - point_end.x);
    } else {
        vector_angle = std::fabs((double)((point_end.y - point_begin.y) / (point_end.x - point_begin.x)));
        vector_angle = vector_angle * 180 / PI;
        if (point_end.y > point_begin.y) {
            if (point_end.x > point_begin.x) {
                vector_angle = vector_angle * -1 + 360;
            } else {
                vector_angle = vector_angle + 180;
            }
        } else {
            if (point_end.x > point_begin.x) {
                vector_angle = vector_angle;
            } else {
                vector_angle = vector_angle * -1 + 180;
            }
        }
        vector_length = std::sqrt((double)(point_begin.x - point_end.x) * (point_begin.x - point_end.x) + 
            (point_begin.y - point_end.y) * (point_begin.y - point_end.y));
    }
}

// get background image
void CHandGestureRecognitionSystemDlg::GetBackgroundImage(void)
{
    image_background = image_camera;
}

// close system

void CHandGestureRecognitionSystemDlg::OnClickedClose()
{
    // TODO: Add your control notification handler code here
    CDialogEx::OnOK();
}
