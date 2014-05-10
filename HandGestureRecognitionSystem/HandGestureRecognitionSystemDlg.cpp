
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
    image_preprocess = Mat(image_height, image_width, CV_8UC3);
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
    ON_BN_CLICKED(IDC_BUTTON_REFRESH_BACKGROUND, &CHandGestureRecognitionSystemDlg::OnClickedButtonRefreshBackground)
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
    // set background
    GetBackgroundImage();
    // start timer;
    StartTimer();
}


void CHandGestureRecognitionSystemDlg::OnClickedButtonStopCamera()
{
    // TODO: Add your control notification handler code here
    StopTimer();
}

void CHandGestureRecognitionSystemDlg::OnClickedButtonRefreshBackground()
{
    // TODO: Add your control notification handler code here
    GetBackgroundImage();
}


void CHandGestureRecognitionSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if(!InitCameraAndImage())
        return;
    // play image camera
    PlayImage(image_camera, IDC_IMAGE_CAMERA, rect_image_camera);
    // process
    image_preprocess = Preprocess(image_camera, image_preprocess);
    // segment
    image_segment = HandSegment(image_preprocess, image_segment);
    PlayImage(image_segment, IDC_IMAGE_SEGMENT, rect_image_segment);
    // feature detect
    image_feature = FeatureDetect(image_segment, image_feature);
    PlayImage(image_feature, IDC_IMAGE_FEATURE, rect_image_feature);
    // recognition
    image_recognition = Recognition(image_feature, image_recognition);
    PlayImage(image_recognition, IDC_IMAGE_RECOGNITION, rect_image_recognition);
#ifdef _DEBUG
    TRACE("image_camera channel = %d\n", image_camera.channels());
    TRACE("image_segment channel = %d\n", image_segment.channels());
    TRACE("image_feature channel = %d\n", image_feature.channels());
    TRACE("image_recognition channel = %d\n", image_recognition.channels());
#endif
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
        image_camera.copyTo(image_segment);
        image_camera.copyTo(image_preprocess);
        image_camera.copyTo(image_feature);
        image_camera.copyTo(image_recognition);
        return true;
    }
}

// hand gesture process

Mat& CHandGestureRecognitionSystemDlg::Preprocess(const Mat& image_input, Mat& image_output)
{
    image_input.copyTo(image_output);
    medianBlur(image_output, image_output, 5);
    //cvtColor(image_input,image_input,CV_BGR2GRAY, 1);
    //GaussianBlur(image_input, image_input, Size(3, 3), 2.5);
    //adaptiveThreshold(image_input, image_input, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 0);
    return image_output;
}

// get background image

void CHandGestureRecognitionSystemDlg::GetBackgroundImage(void)
{
    image_preprocess.copyTo(image_background);
    cvtColor(image_background, image_background, CV_BGR2YCrCb);
}

// hand gesture segment

Mat& CHandGestureRecognitionSystemDlg::HandSegment(const Mat& image_input, Mat& image_output)
{
    image_input.copyTo(image_output);
    Mat* image_temp = new Mat();
    image_input.copyTo(*image_temp);
    // 肤色检测：
    // 1.颜色空间转换
    cvtColor(*image_temp, *image_temp, CV_BGR2YCrCb);
    int channels = (*image_temp).channels();
#ifdef _DEBUG
    TRACE("image_temp channels = %d\n", channels);
    TRACE("image_output channels = %d\n", image_output.channels());
#endif
    // 2.提取肤色
    for (int i = 0; i < (*image_temp).rows; i++) {
        unsigned char* p_temp = (*image_temp).ptr<uchar>(i);
        unsigned char* p_out = image_output.ptr<uchar>(i);
        for (int j = 0; j < (*image_temp).cols; j++) {
            int Cr = *(p_temp + j * channels + 1);
            int Cb = *(p_temp + j * channels + 2);
            if (Cr >= 133 && Cr <= 173 && Cb >= 77 && Cb <= 127) {
                *(p_out + j * channels) = 255;
                *(p_out + j * channels + 1) = 255;
                *(p_out + j * channels + 2) = 255;
            } else {
                *(p_out + j * channels) = 0;
                *(p_out + j * channels + 1) = 0;
                *(p_out + j * channels + 2) = 0;
            }
        }
    }
    delete image_temp;
    // 3.背景差分
    for (int i = 0; i < image_background.rows; i++) {
        unsigned char* p_bg = image_background.ptr<uchar>(i);
        unsigned char* p_out = image_output.ptr<uchar>(i);
        for (int j = 0; j < image_background.cols; j++) {
            int Cr = *(p_bg + j * channels + 1);
            int Cb = *(p_bg + j * channels + 2);
            if (Cr >= 133 && Cr <= 173 && Cb >= 77 && Cb <= 127 && *(p_out + j * channels) == 255) {
                *(p_out + j * channels) = 0;
                *(p_out + j * channels + 1) = 0;
                *(p_out + j * channels + 2) = 0;
            }
        }
    }
    return image_output;
}

// hand gesture feature detect

Mat& CHandGestureRecognitionSystemDlg::FeatureDetect(const Mat& image_input, Mat& image_output)
{
    image_input.copyTo(image_output);
    cvtColor(image_output, image_output, CV_BGR2GRAY);
#ifdef _DEBUG
    TRACE("gray image channels = %d\n", image_output.channels());
#endif
    // morphology
    Mat kernel = Mat(3,3,CV_8UC1, Scalar(255));
    morphologyEx(image_output, image_output, MORPH_OPEN, kernel);
    // floodfill
    int num_floodfill = 0;
    int area_max = 0;
    int value_max = 0;
    for (int i = 0; i < image_output.rows; i++) {
        unsigned char* p_out = image_output.ptr<uchar>(i);
        for (int j = 0; j < image_output.cols; j++) {
        	if (*(p_out + j) == 255) {
                num_floodfill++;
                Scalar new_val = Scalar(num_floodfill);
                Point seed = Point(j, i);
                CRect rect;
                int area = floodFill(image_output,seed, new_val);
                if (area > area_max) {
                    area_max = area;
                    value_max = num_floodfill;
                }
        	}
        }
    }
    // max area
    int area_left = image_output.cols;
    int area_right = 0;
    int area_top = image_output.rows;
    int area_buttom = 0;
    for (int i = 0; i < image_output.rows; i++) {
        unsigned char* p_out = image_output.ptr<uchar>(i);
        for (int j = 0; j < image_output.cols; j++) {
            if (*(p_out + j) == value_max) {
                *(p_out + j) = 255;
                if (area_left > j) area_left = j;
                if (area_right < j) area_right = j;
                if (area_top > i) area_top = i;
                if (area_buttom < i) area_buttom = i;
            } else {
                *(p_out + j) = 0;
            }
        }
    }
#ifdef _DEBUG
    TRACE("area_left = %d\n", area_left);
    TRACE("area_right = %d\n", area_right);
    TRACE("area_top = %d\n", area_top);
    TRACE("area_buttom = %d\n", area_buttom);
#endif
    // rectangle
    rectangle(image_output, Point(area_left, area_top), Point(area_right, area_buttom), Scalar(255), 5); 
    // moment
    Moments moment = moments(image_output);
    int center_x = moment.m10 / moment.m00;
    int center_y = moment.m01 / moment.m00;
    point_end = Point(center_x, center_y);
    circle(image_output, point_end, 10, Scalar(255), 5);
    GetVector(point_begin, point_end);
    if (vector_length >= 20 || point_begin == Point(image_width / 2, image_height / 2)) {
        point_begin = point_end;
    }
#ifdef _DEBUG
    TRACE("vector_length = %d\n", vector_length);
    TRACE("vector_angle = %f\n", vector_angle);
#endif
    return image_output;
}

// hand gesture recognition

Mat& CHandGestureRecognitionSystemDlg::Recognition(const Mat& image_input, Mat& image_output)
{
    //image_camera.copyTo(image_output);
    *(image_output.ptr<uchar>(point_begin.y) + point_begin.x * image_output.channels()) = 255;
    *(image_output.ptr<uchar>(point_begin.y) + point_begin.x * image_output.channels() + 1) = 255;
    *(image_output.ptr<uchar>(point_begin.y) + point_begin.x * image_output.channels() + 2) = 255;
    *(image_output.ptr<uchar>(point_end.y) + point_end.x * image_output.channels()) = 255;
    *(image_output.ptr<uchar>(point_end.y) + point_end.x * image_output.channels() + 1) = 255;
    *(image_output.ptr<uchar>(point_end.y) + point_end.x * image_output.channels() + 2) = 255;
    // draw a line
    line(image_output, point_begin, point_end, CV_RGB(255, 0, 0), 5);
    // put text
    if (vector_length > 20) {
        std::string text = std::string("direction");
        if (vector_angle >= 45 && vector_angle <= 135) {
            text = std::string("up");
        } else if (vector_angle >= 135 && vector_angle <= 225) {
            text = std::string("left");
        } else if (vector_angle >= 225 && vector_angle <= 270) {
            text = std::string("down");
        } else {
            text = std::string("right");
        }
        putText(image_output, text, point_end, FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255, 0, 0), 2);
    }


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

// close system

void CHandGestureRecognitionSystemDlg::OnClickedClose()
{
    // TODO: Add your control notification handler code here
    CDialogEx::OnOK();
}

