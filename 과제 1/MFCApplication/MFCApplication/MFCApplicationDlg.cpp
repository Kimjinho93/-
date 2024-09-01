
// MFCApplicationDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <thread>
#include <random>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplicationDlg 대화 상자



CMFCApplicationDlg::CMFCApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Circle_X, m_xEdit);
	DDX_Control(pDX, IDC_EDIT_Circle_Y, m_yEdit);
	DDX_Control(pDX, IDC_EDIT_Circle_X2, m_xEdit2);
	DDX_Control(pDX, IDC_EDIT_Circle_Y2, m_yEdit2);
	DDX_Control(pDX, IDC_EDIT_Time, m_TimeEdit);
}

BEGIN_MESSAGE_MAP(CMFCApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DrawCircle, &CMFCApplicationDlg::OnBnClickedButtonDrawcircle)
	ON_BN_CLICKED(IDC_BUTTON_Save_Image, &CMFCApplicationDlg::OnBnClickedButtonSaveImage)
	ON_BN_CLICKED(IDC_BUTTON_Load_Image, &CMFCApplicationDlg::OnBnClickedButtonLoadImage)
	ON_BN_CLICKED(IDC_Action, &CMFCApplicationDlg::OnBnClickedAction)
	ON_MESSAGE(WM_USER + 1, &CMFCApplicationDlg::OnUpdateDisplay)
END_MESSAGE_MAP()


// CMFCApplicationDlg 메시지 처리기

BOOL CMFCApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	CString xStr, yStr;
	CString xStr2, yStr2;
	CString TimeStr;

	xStr.Format(_T("0")); 
	yStr.Format(_T("0"));
	xStr2.Format(_T("0"));
	yStr2.Format(_T("0"));
	TimeStr.Format(_T("10"));

	m_xEdit.SetWindowText(xStr);
	m_yEdit.SetWindowText(yStr);
	m_xEdit2.SetWindowText(xStr2);
	m_yEdit2.SetWindowText(yStr2);
	m_TimeEdit.SetWindowText(TimeStr);

	m_nWidth = 0;
	m_nHeight = 0;
	m_nBpp = 0;
	m_nRadius = 0;
	m_nGray = 0;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplicationDlg::OnBnClickedButtonDrawcircle()
{
	std::thread(&CMFCApplicationDlg::ThreadDrawCircle, this).detach();
}

void CMFCApplicationDlg::OnBnClickedButtonSaveImage()
{
	m_image.Save(_T("C:\\Image\\Save.bmp"));
}

void CMFCApplicationDlg::OnBnClickedButtonLoadImage()
{
	std::thread(&CMFCApplicationDlg::ThreadLoadImage, this).detach();
}

void CMFCApplicationDlg::OnBnClickedAction()
{
	std::thread(&CMFCApplicationDlg::ThreadAction, this).detach();
}

void CMFCApplicationDlg::UpdateDisplay()
{
	CClientDC dc(this);
	if (!m_image.IsNull())
	{
		m_image.Draw(dc, 0, 0);
	}
}

void CMFCApplicationDlg::DrawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
	CString TimeStr;
	m_TimeEdit.GetWindowText(TimeStr);

	// 중심 좌표 조정
	int nCenterX = x;
	int nCenterY = y;
	int nPitch = m_image.GetPitch();

	// 이미지 크기와 범위 체크
	int xStart = max(0, nCenterX - nRadius);
	int yStart = max(0, nCenterY - nRadius);
	int xEnd = min(m_nWidth, nCenterX + nRadius);
	int yEnd = min(m_nHeight, nCenterY + nRadius);

	// 원 그리기
	for (int j = yStart; j < yEnd; j++)
	{
		for (int i = xStart; i < xEnd; i++)
		{
			if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
			{
				fm[j * nPitch + i] = nGray;
			}
		}
	}

	// 지연 시간 설정
	Sleep(_ttoi(TimeStr));
}

bool CMFCApplicationDlg::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	double dX = i - nCenterX;
	double dY = j - nCenterY;
	double dDist = dX * dX + dY * dY;

	return dDist < nRadius* nRadius;
}

CString CMFCApplicationDlg::GetImageFolderPath()
{
	TCHAR szPath[MAX_PATH];
	CString folderPath;

	// 실행 파일의 디렉토리 경로를 가져옵니다.
	if (GetModuleFileName(NULL, szPath, MAX_PATH) != 0)
	{
		// 디렉토리 경로만 추출합니다.
		CString exePath = szPath;
		int pos = exePath.ReverseFind('\\');
		if (pos != -1)
		{
			exePath = exePath.Left(pos);
			folderPath.Format(_T("%s\\image\\"), exePath);

			// 디렉토리 생성
			if (CreateDirectory(folderPath, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
			{
				return folderPath;
			}
		}
	}
	return _T("");
}

void CMFCApplicationDlg::ThreadDrawCircle()
{
	// 스레드에서 이미지 생성 및 원 그리기
	CRect rect;
	GetWindowRect(&rect);

	m_nWidth = rect.Width();
	m_nHeight = rect.Height() - 150;
	m_nBpp = 8;

	// 스레드 안전한 난수 생성기 설정
	std::random_device rd; // 진짜 난수 생성기 (하드웨어 기반)
	std::mt19937 gen(rd()); // Mersenne Twister 엔진
	std::uniform_int_distribution<> dis(10, 100); // 범위: 10 ~ 100
	m_nRadius = dis(gen);

	m_nGray = 0;

	if (!m_image.IsNull())
	{
		m_image.Destroy();
	}

	m_image.Create(m_nWidth, -m_nHeight, m_nBpp);

	if (m_nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
		{
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0xff, nPitch * abs(m_nHeight));

	CString xStr, yStr;
	m_xEdit.GetWindowText(xStr);
	m_yEdit.GetWindowText(yStr);
	int x = _ttoi(xStr);
	int y = _ttoi(yStr);

	DrawCircle(fm, x, y, m_nRadius, m_nGray);

	// UI 스레드에서 화면 업데이트
	PostMessage(WM_USER + 1, 0, 0);
}

void CMFCApplicationDlg::ThreadLoadImage()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("BMP Files (*.bmp)|*.bmp|JPEG Files (*.jpg;*.jpeg)|*.jpg;*.jpeg|All Files (*.*)|*.*||"), this);

	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();
		TRACE(_T("Selected file path: %s\n"), filePath);

		// 이미지가 이미 존재하는 경우 먼저 파괴
		if (!m_image.IsNull())
		{
			m_image.Destroy();
		}

		// 이미지 로드
		HRESULT hr = m_image.Load(filePath);
		if (FAILED(hr))
		{
			CString errorMsg;
			errorMsg.Format(_T("이미지 로드 실패. HRESULT: 0x%08X"), hr);
			AfxMessageBox(errorMsg);
			return;
		}

		// 이미지가 정상적으로 로드되었는지 확인
		if (m_image.GetWidth() > 0 && m_image.GetHeight() > 0)
		{
			m_nWidth = m_image.GetWidth();
			m_nHeight = m_image.GetHeight();
			m_nBpp = m_image.GetBPP();

			// 다이얼로그에 이미지 출력
			UpdateDisplay();

			// 검은색 픽셀 찾기
			BYTE* pBits = (BYTE*)m_image.GetBits();
			int pitch = m_image.GetPitch(); // 한 줄의 바이트 수
			std::vector<CPoint> blackPixels;
			blackPixels.clear();

			// 이미지에서 검은색 픽셀 찾기
			for (int y = 0; y < m_nHeight; ++y)
			{
				for (int x = 0; x < m_nWidth; ++x)
				{
					// 8비트 그레이스케일 이미지의 경우, 픽셀 데이터는 1바이트로 접근
					BYTE pixelValue = pBits[y * pitch + x];

					// 검은색 픽셀인지 확인 (값이 0인 경우)
					if (pixelValue == 0)
					{
						blackPixels.push_back(CPoint(x, y));
					}
				}
			}

			if (blackPixels.empty())
			{
				AfxMessageBox(_T("검은색 픽셀이 없습니다."));
				return;
			}

			// 검은색 픽셀의 평균 위치를 계산하여 중심점 찾기
			double sumX = 0;
			double sumY = 0;

			for (const auto& pt : blackPixels)
			{
				sumX += pt.x;
				sumY += pt.y;
			}

			int centerX = static_cast<int>(sumX / blackPixels.size());
			int centerY = static_cast<int>(sumY / blackPixels.size());

			// 다이얼로그에 중심점을 표시하기 위해 화면에 X 모양을 그리는 코드 추가
			CClientDC dc(this);

			// X 모양 그리기
			CPen penX(PS_SOLID, 2, RGB(255, 0, 0)); // 빨강색
			dc.SelectObject(&penX);

			dc.MoveTo(centerX - 10, centerY - 10);
			dc.LineTo(centerX + 10, centerY + 10);

			dc.MoveTo(centerX - 10, centerY + 10);
			dc.LineTo(centerX + 10, centerY - 10);

			// 좌표값을 텍스트로 표시
			CString text;
			text.Format(_T("(%d, %d)"), centerX, centerY);

			CFont font;
			font.CreatePointFont(100, _T("Arial"));
			CFont* pOldFont = dc.SelectObject(&font);

			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(0, 255, 0));

			dc.TextOut(centerX + 15, centerY - 10, text);

			dc.SelectObject(pOldFont);
		}
		else
		{
			AfxMessageBox(_T("이미지가 제대로 로드되지 않았습니다."));
		}
	}
}

void CMFCApplicationDlg::ThreadAction()
{
	// 변수 초기화
	CString xStr1, yStr1, xStr2, yStr2, TimeStr;
	m_xEdit.GetWindowText(xStr1);
	m_yEdit.GetWindowText(yStr1);
	m_xEdit2.GetWindowText(xStr2);
	m_yEdit2.GetWindowText(yStr2);
	m_TimeEdit.GetWindowText(TimeStr);

	int x1 = _ttoi(xStr1);
	int y1 = _ttoi(yStr1);
	int x2 = _ttoi(xStr2);
	int y2 = _ttoi(yStr2);
	int delay = _ttoi(TimeStr);

	// 이동 범위 및 방향 계산
	int dx = x2 - x1;
	int dy = y2 - y1;
	int steps = max(abs(dx), abs(dy));

	double stepX = static_cast<double>(dx) / steps;
	double stepY = static_cast<double>(dy) / steps;

	// 현재 위치 초기화
	double currentX = x1;
	double currentY = y1;

	// 이동 루프
	for (int i = 0; i <= steps; i++)
	{
		// 이미지 객체 초기화
		if (!m_image.IsNull())
		{
			m_image.Destroy();
		}

		m_image.Create(m_nWidth, -m_nHeight, m_nBpp);

		// 컬러 테이블 설정 (8비트 그레이스케일)
		if (m_nBpp == 8)
		{
			static RGBQUAD rgb[256];
			for (int i = 0; i < 256; i++)
			{
				rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
			}
			m_image.SetColorTable(0, 256, rgb);
		}

		// 이미지 데이터를 가져오고 메모리를 초기화
		int nPitch = m_image.GetPitch();
		unsigned char* fm = (unsigned char*)m_image.GetBits();
		memset(fm, 0xff, nPitch * abs(m_nHeight));  // 이미지를 흰색으로 초기화

		// 원 그리기
		DrawCircle(fm, static_cast<int>(round(currentX)), static_cast<int>(round(currentY)), m_nRadius, m_nGray);

		// UI 스레드에서 화면 업데이트
		PostMessage(WM_USER + 1, 0, 0);

		// 이미지 파일 경로 설정
		CString filePath;
		filePath.Format(_T("%sframe_%03d.bmp"), GetImageFolderPath(), i);

		// 이미지 저장
		if (!m_image.IsNull())
		{
			m_image.Save(filePath);
		}


		// 다음 위치로 이동
		currentX += stepX;
		currentY += stepY;

		// 지연 시간 설정
		Sleep(delay);
	}
}

LRESULT CMFCApplicationDlg::OnUpdateDisplay(WPARAM wParam, LPARAM lParam)
{
	UpdateDisplay();
	return 0;
}