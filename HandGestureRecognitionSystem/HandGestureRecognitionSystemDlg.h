
// HandGestureRecognitionSystemDlg.h : header file
//

// CHandGestureRecognitionSystemDlg dialog
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
    afx_msg void OnClickedButtonreadimage();
    afx_msg void OnClickedButtonprocessimage();
private:
    Mat image_src;
    Mat image_dst;
    int image_height;
    int image_width;
    int image_interval;
};
