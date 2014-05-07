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
    Mat image_src;
    Mat image_dst;
    int image_height;
    int image_width;
    int image_interval;
    VideoCapture videocapture;
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    void StartTimer(void);
    void StopTimer(void);
private:
    CRect rect_image_src;
    CRect rect_image_dst;
public:
    void PlayImageSrc(void);
    void PlayImageDst(void);
    void ProcessImage(void);
    bool GetImageSrc(void);
};
