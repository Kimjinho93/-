
// MFCApplicationDlg.h: 헤더 파일
//

#pragma once


// CMFCApplicationDlg 대화 상자
class CMFCApplicationDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplicationDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	CImage m_image;
	CEdit m_xEdit, m_yEdit; //Start X,Y
	CEdit m_xEdit2, m_yEdit2; //End X,Y
	CEdit m_TimeEdit;

	int m_nWidth;
	int m_nHeight;
	int m_nRadius;
	int m_nBpp;
	int m_nGray;

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDrawcircle();
	afx_msg void OnBnClickedButtonSaveImage();
	afx_msg void OnBnClickedButtonLoadImage();
	afx_msg void OnBnClickedAction();
	void UpdateDisplay();
	void DrawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
	bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	CString GetImageFolderPath();

private:
	void ThreadDrawCircle();
	void ThreadLoadImage();
	void ThreadAction();
	afx_msg LRESULT OnUpdateDisplay(WPARAM wParam, LPARAM lParam);
};
