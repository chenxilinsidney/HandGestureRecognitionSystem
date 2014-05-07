// HandGestureRecognitionSystemDlg.h : header file
//

// HEADER FILE FOR IMAGE PROCESSING
#include "Header.h"

// CHandGestureRecognitionSystemDlg dialog
#include "atltypes.h"
class CHandGestureRecognitionSystemDlg : public CDialogEx
{
// Construction
public:
	CHandGestureRecognitionSystemDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HANDGESTURERECOGNITIONSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClickedButtonPlayCamera();
    afx_msg void OnClickedButtonStopCamera();
private:
    // camera
    VideoCapture videocapture;
    // image
    Mat image_camera;
    Mat image_segment;
    Mat image_feature;
    Mat image_recognition;
    Mat image_background;
    // image parameter
    int image_height;
    int image_width;
    int image_interval;
    // image rect for show
    CRect rect_image_camera;
    CRect rect_image_segment;
    CRect rect_image_feature;
    CRect rect_image_recognition;
    // feature point
    Point point_begin;
    Point point_end;
    double vector_angle;
    double vector_length;
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    void StartTimer(void);
    void StopTimer(void);
private:
    bool InitCameraAndImage(void);
    void GetBackgroundImage(void);
    Mat& HandSegment(const Mat& image_input, Mat& image_output);
    Mat& FeatureDetect(const Mat& image_input, Mat& image_output);
    Mat& Recognition(const Mat& image_input, Mat& image_output);
    void GetVector(Point point_begin, Point point_end);
    void PlayImage(Mat& image, int ID, CRect& rect);
public:
    afx_msg void OnClickedClose();
};
